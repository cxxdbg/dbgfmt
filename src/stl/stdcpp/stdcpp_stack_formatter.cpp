
/// \file stdcpp_stack_formatter.cpp
/// Contains implementation of the stdc++ stack formatter.

#include "stdcpp_stack_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "../../proxy_fmt_result.hpp"


namespace dbgfmt {


bool stdcpp_stack_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::stack";
}


fmt_result_ref stdcpp_stack_formatter::format(const value & val) {
    // formatting deque
    auto d = ctx().format(val["c"]);

    // overriding type name in formatting result
    return make_fmt_result<ot_proxy_fmt_result>(d, val.wqual_type_name());
}


}
