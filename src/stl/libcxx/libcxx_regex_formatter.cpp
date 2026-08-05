
/// \file libcxx_regex_formatter.cpp
/// Contains implementation of formatter classes for libc++ regex types.

#include "libcxx_regex_formatter.hpp"
#include "libcxx_vector_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "../../proxy_fmt_result.hpp"
#include "libcxx_vector_formatter.hpp"


namespace dbgfmt {


bool libcxx_sub_match_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::__1::sub_match";
}


fmt_result_ref libcxx_sub_match_formatter::transform_result(const value & val, const fmt_result_ref & res) {
    auto proxy = std::make_unique<proxy_fmt_result>(res);
    proxy->add_begin_child(L"[begin]", [&ctx = ctx(), val] {
        return ctx.format(val["first"]);
    });
    proxy->add_begin_child(L"[end]", [&ctx = ctx(), val] {
        return ctx.format(val["second"]);
    });

    return make_ref(std::move(proxy));
}


std::size_t libcxx_sub_match_formatter::size(const value & val) const {
    auto begin = val["first"]["__i"];
    auto end = val["second"]["__i"];
    return static_cast<std::size_t>(end - begin);
}


value libcxx_sub_match_formatter::element(const value & val, std::size_t index) const {
    auto begin = val["first"]["__i"];
    return *(begin + index);
}



bool libcxx_match_results_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::__1::match_results";
}


std::size_t libcxx_match_results_formatter::size(const value & val) const {
    return get_libcxx_vector_size(val["__matches_"]);
}


value libcxx_match_results_formatter::element(const value & val, std::size_t index) const {
    return get_libcxx_vector_element(val["__matches_"], index);
}


}
