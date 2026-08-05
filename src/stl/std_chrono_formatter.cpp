
/// \file std_chrono_formatter.cpp
/// Contains implementation of formatters for the std::chrono library

#include "std_chrono_formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include <cxxtn/type_name_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <memory>
#include <sstream>


namespace dbgfmt {

#if __cplusplus > 201703L
using chrono_days = std::chrono::days;
using chrono_weeks = std::chrono::weeks;
using chrono_months = std::chrono::months;
using chrono_years = std::chrono::years;
#else
using chrono_days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
using chrono_weeks = std::chrono::duration<int, std::ratio_multiply<std::ratio<7>, chrono_days::period>>;
using chrono_years = std::chrono::duration<int, std::ratio_multiply<std::ratio<146097, 400>, chrono_days::period>>;
using chrono_months = std::chrono::duration<int, std::ratio_divide<chrono_years::period, std::ratio<12>>>;
#endif


/// Returns duration representation name
static auto duration_rep_name(const std::chrono::nanoseconds &) { return L"nanosecond"; }
static auto duration_rep_name(const std::chrono::microseconds &) { return L"microsecond"; }
static auto duration_rep_name(const std::chrono::milliseconds &) { return L"millisecond"; }
static auto duration_rep_name(const std::chrono::seconds &) { return L"second"; }
static auto duration_rep_name(const std::chrono::minutes &) { return L"minute"; }
static auto duration_rep_name(const std::chrono::hours &) { return L"hour"; }
static auto duration_rep_name(const chrono_days &) { return L"day"; }
static auto duration_rep_name(const chrono_weeks &) { return L"week"; }
static auto duration_rep_name(const chrono_months &) { return L"month"; }
static auto duration_rep_name(const chrono_years &) { return L"year"; }


/// Returns duration representation short name
static auto duration_rep_short_name(const std::chrono::nanoseconds &) { return L"ns"; }
static auto duration_rep_short_name(const std::chrono::microseconds &) { return L"us"; }
static auto duration_rep_short_name(const std::chrono::milliseconds &) { return L"ms"; }
static auto duration_rep_short_name(const std::chrono::seconds &) { return L"s"; }
static auto duration_rep_short_name(const std::chrono::minutes &) { return L"m"; }
static auto duration_rep_short_name(const std::chrono::hours &) { return L"h"; }
static auto duration_rep_short_name(const chrono_days &) { return L"d"; }
static auto duration_rep_short_name(const chrono_weeks &) { return L"w"; }
static auto duration_rep_short_name(const chrono_months &) { return L"M"; }
static auto duration_rep_short_name(const chrono_years &) { return L"y"; }


/// Extracts duration value using specified duration type
template <typename ResultDuration, typename Duration>
static long long get_duration_value(const Duration & d) {
    return std::chrono::duration_cast<ResultDuration>(d).count();
}


/// Extracts normalized duration component
template <typename ResultDuration, typename NormDuration, typename Duration>
static long long get_duration_component(const Duration & d) {
    auto comp = std::chrono::duration_cast<ResultDuration>(d);

    // normalizing duration component (extracting value of previous component)
    if constexpr (!std::is_same_v<NormDuration, void>) {
        auto prev_comp = std::chrono::duration_cast<NormDuration>(d);
        auto prev_comp_casted = std::chrono::duration_cast<ResultDuration>(prev_comp);
        comp -= prev_comp_casted;
    }

    return comp.count();
}


/// Writes normalized duration component to output stream
template <typename ResultDuration, typename NormDuration, typename Duration>
static void write_duration_component(std::wostream & str, bool & first, const Duration & d) {
    auto val = get_duration_component<ResultDuration, NormDuration>(d);
    if (val == 0) {
        return;
    }

    if (!first) {
        str << L' ';
    } else {
        first = false;
    }

    str << val << duration_rep_short_name(ResultDuration{});
}


/// Writes normalized duration to output stream
template <typename Duration>
static void write_normalized_duration(std::wostream & str, const Duration & d) {
    bool first = true;
    write_duration_component<chrono_years, void>(str, first, d);
    write_duration_component<chrono_months, chrono_years>(str, first, d);
    write_duration_component<chrono_days, chrono_months>(str, first, d);
    write_duration_component<std::chrono::hours, chrono_days>(str, first, d);
    write_duration_component<std::chrono::minutes, std::chrono::hours>(str, first, d);
    write_duration_component<std::chrono::seconds, std::chrono::minutes>(str, first, d);
    write_duration_component<std::chrono::milliseconds, std::chrono::seconds>(str, first, d);
    write_duration_component<std::chrono::microseconds, std::chrono::milliseconds>(str, first, d);
    write_duration_component<std::chrono::nanoseconds, std::chrono::microseconds>(str, first, d);
}


/// Returns string containing normalized duration
template <typename Duration>
static std::wstring get_normalized_duration(const Duration & d) {
    std::wostringstream str;
    write_normalized_duration(str, d);
    return str.str();
}


/// Formats duration value using specified period
template <typename ResultDuration, typename Duration>
static fmt_result_ref format_duration_value(const Duration & d) {
    return make_fmt_result<fixed_fmt_result>(std::to_wstring(get_duration_value<ResultDuration>(d)),
                                             std::wstring{},
                                             source_position{},
                                             source_position{});
}


/// Formats duration
template <typename Duration>
static fmt_result_ref format_duration(context & ctx, const Duration & d, const value & val, const value & rep_val) {
    std::wostringstream str;
    str << d.count() << duration_rep_short_name(d);
    auto norm_d = get_normalized_duration(d);
    if (str.str() != norm_d) {
        str << L" (" << norm_d << L')';
    }

    auto res = std::make_unique<fixed_fmt_result>(str.str(),
                                                  val.wqual_type_name(),
                                                  source_position{},
                                                  val.type().def_pos());

    res->add_child(L"[rep]", [&ctx, rep_val]() -> fmt_result_ref {
        return ctx.format(rep_val);
    });

    res->add_child(L"[ratio]", [d] {
        using period = typename Duration::period;
        std::wostringstream str;
        str << period::num;
        if (period::den != 1) {
            str << L" / " << period::den;
        }
        str << L" (" << duration_rep_name(d) << L"s)";
        return make_fmt_result<fixed_fmt_result>(str.str(), std::wstring{}, source_position{}, source_position{});
    });

    res->add_child(L"[normalized]", [d] {
        std::wostringstream str;
        write_normalized_duration(str, d);
        return make_fmt_result<fixed_fmt_result>(str.str(), std::wstring{}, source_position{}, source_position{});
    });

    res->add_child(L"[years]", [d] { return format_duration_value<chrono_years>(d); });
    res->add_child(L"[months]", [d] { return format_duration_value<chrono_months>(d); });
    res->add_child(L"[days]", [d] { return format_duration_value<chrono_days>(d); });
    res->add_child(L"[hours]", [d] { return format_duration_value<std::chrono::hours>(d); });
    res->add_child(L"[minutes]", [d] { return format_duration_value<std::chrono::minutes>(d); });
    res->add_child(L"[seconds]", [d] { return format_duration_value<std::chrono::seconds>(d); });
    res->add_child(L"[milliseconds]", [d] { return format_duration_value<std::chrono::milliseconds>(d); });
    res->add_child(L"[microseconds]", [d] { return format_duration_value<std::chrono::microseconds>(d); });
    res->add_child(L"[nanoseconds]", [d] { return format_duration_value<std::chrono::nanoseconds>(d); });

    return make_ref(std::move(res));
}


/// Converts duration representation raw values to std::duration and passes it to specified functor
template <typename Functor>
static auto make_duration_and_apply(long long rep, long long num, long long denom, Functor f) {
    if (num == 1 && denom == 1000000000) {
        return f(std::chrono::nanoseconds{rep});
    } else if (num == 1 && denom == 1000000) {
        return f(std::chrono::microseconds{rep});
    } else if (num == 1 && denom == 1000) {
        return f(std::chrono::milliseconds{rep});
    } else if (num == 1 && denom == 1) {
        return f(std::chrono::seconds{rep});
    } else if (num == 60 && denom == 1) {
        return f(std::chrono::minutes{rep});
    } else if (num == 3600 && denom == 1) {
        return f(std::chrono::hours{rep});
    } else if (num == 86400 && denom == 1) {
        return f(chrono_days{rep});
    } else if (num == 604800 && denom == 1) {
        return f(chrono_days{rep});
    } else if (num == 2629746 && denom == 1) {
        return f(chrono_days{rep});
    } else if (num == 31556952 && denom == 1) {
        return f(chrono_years{rep});
    } else {
        std::ostringstream msg;
        msg << "unknown ratio num and denom (" << num << " / " << denom << ") for std::duration";
        throw format_error{msg.str()};
    }
}


/// Returns duration representation field
static value get_duration_rep_field(const value & d, bool is_libcxx) {
    auto rep_fld_name = is_libcxx ? "__rep_" : "__r";
    return d[rep_fld_name];
}


/// Converts duration representation fmt value to duration and passes it to specified functor
template <typename Functor>
static auto make_duration_and_apply(const value & val, bool is_libcxx, Functor f) {
    // parsing type name
    std::istringstream tn_istr{val.qual_type_name()};
    auto type_name = cxxtn::parse_type_name(tn_istr);
    if (!type_name.is_valid()) {
        std::ostringstream msg;
        msg << "can't parse std::duration type name: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }


    // extracting duration ratio from template parameters

    auto tmpl_type_name = dynamic_cast<const cxxtn::template_type_name*>(type_name.type());
    if (!tmpl_type_name) {
        std::ostringstream msg;
        msg << "std::duration type name is not a template: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    if (tmpl_type_name->params().size() < 2) {
        std::ostringstream msg;
        msg << "invalid number of template paramters for std::duration type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    auto ratio_type_name = tmpl_type_name->params()[1];
    if (!ratio_type_name.is_valid()) {
        std::ostringstream msg;
        msg << "invalid 2nd template parameter for std::duration type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    auto ratio_tmpl_type_name = dynamic_cast<const cxxtn::template_type_name*>(ratio_type_name.type());
    if (!ratio_tmpl_type_name) {
        std::ostringstream msg;
        msg << "ratio parameter is not a std::ratio for std::duration type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    if (ratio_tmpl_type_name->params().size() < 2) {
        std::ostringstream msg;
        msg << "invalid number of template paramters for std::ratio parameter in "
               "std::duration type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    auto ratio_num_type_name = dynamic_cast<const cxxtn::basic_type_name*>(ratio_tmpl_type_name->params()[0].type());
    if (!ratio_num_type_name) {
        std::ostringstream msg;
        msg << "ratio Num parameter is invalid for std::duration type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    auto ratio_denom_type_name = dynamic_cast<const cxxtn::basic_type_name*>(ratio_tmpl_type_name->params()[1].type());
    if (!ratio_denom_type_name) {
        std::ostringstream msg;
        msg << "ratio Denom parameter is invalid for std::duration type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    long long ratio_num = std::stoll(ratio_num_type_name->identifier());
    if (ratio_num == 0) {
        std::ostringstream msg;
        msg << "invalid ratio Num '" << ratio_num << "' parsed from std::duration type '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    long long ratio_denom = std::stoll(ratio_denom_type_name->identifier());
    if (ratio_denom == 0) {
        std::ostringstream msg;
        msg << "invalid ratio Denom '" << ratio_denom << "' parsed from std::duration type '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    // reading representation value
    auto rep_val = get_duration_rep_field(val, is_libcxx);

    // creating duration and passing it to functor
    return make_duration_and_apply(rep_val.as_int64(), ratio_num, ratio_denom, f);
}


/// Converts std::chrono::time_point to boost posix time
/// This code is from the following boost commit:
/// https://lists.boost.org/boost-commit/2009/04/15209.php
template <typename Clock, typename Duration>
static boost::posix_time::ptime time_point_to_ptime(const std::chrono::time_point<Clock, Duration> & from)  { 
    typedef std::chrono::time_point<Clock, Duration> time_point_t; 
    typedef std::chrono::nanoseconds duration_t; 
    typedef duration_t::rep rep_t; 
    rep_t d = std::chrono::duration_cast<duration_t>(from.time_since_epoch()).count(); 
    rep_t sec = d/1000000000; 
    rep_t nsec = d%1000000000; 
    return boost::posix_time::from_time_t(0)+ 
           boost::posix_time::seconds(static_cast<long>(sec))+ 
#ifdef BOOST_DATE_TIME_HAS_NANOSECONDS 
           boost::posix_time::nanoseconds(nsec); 
#else 
           boost::posix_time::microseconds((nsec+500)/1000); 
#endif 
}


/// Formats tm struct and writes it to output stream, appeding fractional seconds obtained from duration
template <typename Duration>
static void format_tm(std::wostream & str, const std::tm & tm, const Duration & d) {
    // writing date and time
    str << (tm.tm_year + 1900) << L'-'
        << std::setfill(L'0') << std::setw(2) << (tm.tm_mon + 1) << L'-'
        << std::setfill(L'0') << std::setw(2) << (tm.tm_mday) << L' '
        << std::setfill(L'0') << std::setw(2) << (tm.tm_hour) << L':'
        << std::setfill(L'0') << std::setw(2) << (tm.tm_min) << L':'
        << std::setfill(L'0') << std::setw(2) << (tm.tm_sec);

    // writing fractional seconds
    auto ns_d = std::chrono::duration_cast<std::chrono::nanoseconds>(d);
    str << L'.' << std::setfill(L'0') << std::setw(9) << (ns_d.count() % 1000000000);
}


bool std_chrono_duration_formatter::accepts(const value & v) const {
    if (!v.is_record()) {
        return false;
    }

    return v.template_name() == "std::chrono::duration" ||
           v.template_name() == "std::__1::chrono::duration";
}


fmt_result_ref std_chrono_duration_formatter::format(const value & val) {
    bool is_libcxx = val.template_name() == "std::__1::chrono::duration";

    // reading representation value
    auto rep_val = get_duration_rep_field(val, is_libcxx);

    // formatting duration
    return make_duration_and_apply(val, is_libcxx, [&ctx = ctx(), &val, &rep_val](auto && d) {
        return format_duration(ctx, d, val, rep_val);
    });
}


bool std_chrono_time_point_formatter::accepts(const value & v) const {
    if (!v.is_record()) {
        return false;
    }

    return v.template_name() == "std::chrono::time_point" ||
           v.template_name() == "std::__1::chrono::time_point";
}


fmt_result_ref std_chrono_time_point_formatter::format(const value & val) {
    bool is_libcxx = val.template_name() == "std::__1::chrono::time_point";

    // parsing type name
    std::istringstream tn_istr{val.qual_type_name()};
    auto type_name = cxxtn::parse_type_name(tn_istr);
    if (!type_name.is_valid()) {
        std::ostringstream msg;
        msg << "can't parse std::duration type name: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }


    // getting type of clock (Value of the Clock template parameter)

    auto tmpl_type_name = dynamic_cast<const cxxtn::template_type_name*>(type_name.type());
    if (!tmpl_type_name) {
        std::ostringstream msg;
        msg << "std::time_point type name is not a template: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    if (tmpl_type_name->params().size() < 2) {
        std::ostringstream msg;
        msg << "invalid number of template paramters for std::time_point type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    auto clock_type_name = dynamic_cast<const cxxtn::basic_type_name*>(tmpl_type_name->params()[0].type());
    if (!clock_type_name) {
        std::ostringstream msg;
        msg << "invalid 1st template parameter for std::time_point type: '" << val.qual_type_name() << "'";
        throw format_error{msg.str()};
    }

    // getting duration field
    const auto * dur_field_name = is_libcxx ? "__d_" : "__d";
    auto dur_value = val[dur_field_name];

    if (clock_type_name->identifier() != "system_clock") {
        // generic case for any clock

        // formatting duration field
        auto dur_fmt_value = ctx().format(dur_value);

        // formatting main value
        std::wostringstream main_val_str;
        main_val_str << L"epoch";
        if (get_duration_rep_field(dur_value, is_libcxx).as_int64() > 0) {
            main_val_str << L'+';
        }
        main_val_str << dur_fmt_value.val();

        // creating formatting result
        auto res = std::make_unique<fixed_fmt_result>(main_val_str.str(),
                                                      val.wqual_type_name(),
                                                      source_position{},
                                                      val.type().def_pos());

        // adding [duration] field to result
        res->add_child(L"[duration]", [&ctx = ctx(), dur_value]() -> fmt_result_ref {
            return ctx.format(dur_value);
        });

        return make_ref(std::move(res));
    }


    // special case for system clock

    std::wostringstream main_val_str;
    make_duration_and_apply(dur_value, is_libcxx, [&main_val_str](const auto & d) {
        std::chrono::time_point<std::chrono::system_clock, std::decay_t<decltype(d)>> tp{d};
        auto sys_tp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp);
        auto time_t_val = std::chrono::system_clock::to_time_t(sys_tp);
        auto tm_val = std::localtime(&time_t_val);
        format_tm(main_val_str, *tm_val, tp.time_since_epoch());
    });

    // creating formatting result
    auto res = std::make_unique<fixed_fmt_result>(main_val_str.str(),
                                                  val.wqual_type_name(),
                                                  source_position{},
                                                  val.type().def_pos());

    // creating [UTC] child
    res->add_child(L"[UTC]", [dur_value, is_libcxx] {
        return make_duration_and_apply(dur_value, is_libcxx, [](const auto & d) {
            std::chrono::time_point<std::chrono::system_clock, std::decay_t<decltype(d)>> tp{d};
            auto sys_tp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp);
            auto time_t_val = std::chrono::system_clock::to_time_t(sys_tp);
            auto tm_val = std::gmtime(&time_t_val);
            std::wostringstream str;
            format_tm(str, *tm_val, tp.time_since_epoch());
            return make_fmt_result<fixed_fmt_result>(str.str(), std::wstring{}, source_position{}, source_position{});
        });
    });

    // creating duration child
    res->add_child(L"[duration]", [dur_value, is_libcxx, &ctx = ctx()] {
        return ctx.format(dur_value);
    });

    // creating date components
    make_duration_and_apply(dur_value, is_libcxx, [&ctx = ctx(), &res](const auto & d) {
        std::chrono::time_point<std::chrono::system_clock, std::decay_t<decltype(d)>> tp{d};
        auto sys_tp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp);
        auto time_t_val = std::chrono::system_clock::to_time_t(sys_tp);
        auto tm = std::localtime(&time_t_val);

        res->add_child(L"[year]", [y = tm->tm_year] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(y + 1900)); });
        res->add_child(L"[month]", [m = tm->tm_mon] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(m + 1)); });
        res->add_child(L"[day]", [d = tm->tm_mday] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(d)); });
        res->add_child(L"[hour]", [h = tm->tm_hour] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(h)); });
        res->add_child(L"[minute]", [m = tm->tm_min] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(m)); });
        res->add_child(L"[second]", [s = tm->tm_sec] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(s)); });

        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() % 1000;
        res->add_child(L"[millisecond]", [ms] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(ms)); });

        long long us = std::chrono::duration_cast<std::chrono::microseconds>(d).count() % 1000;
        res->add_child(L"[microsecond]", [us] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(us)); });

        long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(d).count() % 1000;
        res->add_child(L"[nanosecond]", [ns] { return make_fmt_result<fixed_fmt_result>(std::to_wstring(ns)); });
    });

    return make_ref(std::move(res));
}


}
