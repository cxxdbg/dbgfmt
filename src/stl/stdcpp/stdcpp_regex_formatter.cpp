
/// \file stdcpp_regex_formatter.cpp
/// Contains implementation of formatter classes for libstdc++ regex types.

#include "stdcpp_regex_formatter.hpp"
#include "stdcpp_vector_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "../../proxy_fmt_result.hpp"


namespace dbgfmt {


bool stdcpp_sub_match_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::__cxx11::sub_match";
}


fmt_result_ref stdcpp_sub_match_formatter::transform_result(const value & val, const fmt_result_ref & res) {
    auto proxy = std::make_unique<proxy_fmt_result>(res);
    proxy->add_begin_child(L"[begin]", [&ctx = ctx(), val] {
        return ctx.format(val["first"]);
    });
    proxy->add_begin_child(L"[end]", [&ctx = ctx(), val] {
        return ctx.format(val["second"]);
    });

    return make_ref(std::move(proxy));
}


std::size_t stdcpp_sub_match_formatter::size(const value & val) const {
    auto begin = val["first"]["_M_current"];
    auto end = val["second"]["_M_current"];
    return static_cast<std::size_t>(end - begin);
}


value stdcpp_sub_match_formatter::element(const value & val, std::size_t index) const {
    auto begin = val["first"]["_M_current"];
    return *(begin + index);
}



bool stdcpp_match_results_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::__cxx11::match_results";
}


std::size_t stdcpp_match_results_formatter::size(const value & val) const {
    return get_stdcpp_vector_size(val) - 3;
}


value stdcpp_match_results_formatter::element(const value & val, std::size_t index) const {
    return get_stdcpp_vector_element(val, index);
}


}
