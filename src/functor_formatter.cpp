
/// \file functor_formatter.cpp
/// Contains implementation of the functor_formatter class.

#include "functor_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fmt_result.hpp"
#include "log.hpp"
#include "proxy_fmt_result.hpp"
#include "dbgfmt/format_error.hpp"
#include <algorithm>
#include <sstream>


namespace dbgfmt {


bool functor_formatter::accepts(const value & v) const {
    return ctx().fmt_opts().rec_as_functor() &&
           v.is_record() &&
           get_call_operator(v).is_valid();
}


fmt_result_ref functor_formatter::format(const value & val) {
    // formatting lambda function pointer for val/hexval
    DBGFMT_LOG_TRACE << "functor_formatter::get_call_operator begin";
    auto fptr = get_call_operator(val);
    DBGFMT_LOG_TRACE << "functor_formatter::get_call_operator end";

    if (!fptr.is_valid()) {
        std::ostringstream msg;
        msg << "Can't find operator() in type '" << val.type_name() << "'";
        throw format_error{msg.str()};
    }

    auto ffptr = ctx().format(fptr);

    // formatting lambda as record and creating proxy formatter
    auto res = record_formatter::format(val);
    auto proxy_res = std::make_unique<ov_proxy_fmt_result>(res,
                                                           ffptr.val(),
                                                           ffptr.val_pos());
    proxy_res->add_begin_child(L"[target]", [this, ffptr] { return ffptr; });
    return make_ref(std::move(proxy_res));
}


value functor_formatter::get_call_operator(const value & v) {
    // lambda type may contain serveral instantiations of operator()
    // in case of template lambdas (lambdas with auto parameters).
    // For now, we are looking for the first member functions
    // with name starting with "operator()"

    auto all_mem_funcs = v.mem_funcs();
    auto res = std::find_if(all_mem_funcs.begin(), all_mem_funcs.end(),
    [](const auto & f) {
        static const std::string call_op_name = "operator()";
        return f.compare(0, call_op_name.size(), call_op_name) == 0;
    });

    if (res == all_mem_funcs.end()) {
        return {};
    }

    return v.mem_fun_ptr(*res);
}


}
