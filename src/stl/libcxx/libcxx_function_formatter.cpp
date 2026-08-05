
/// \file libcxx_function_formatter.cpp
/// Contains implementation of formatter for libc++ function.

#include "libcxx_function_formatter.hpp"
#include "libcxx_ns_utils.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/context.hpp"
#include "../../proxy_fmt_result.hpp"
#include <cxxtn/tn.hpp>
#include <memory>
#include <iostream>


namespace dbgfmt {


bool libcxx_function_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "function");
}


fmt_result_ref libcxx_function_formatter::format(const value & val) {
    // getting pointer to __func object
    auto func_obj = val["__f_"]["__f_"];

    std::unique_ptr<fixed_fmt_result> res;

    // checking for empty function
    if (func_obj.as_uint64() == 0) {
        res = std::make_unique<fixed_fmt_result>(L"[functor = <empty>]",
                                                      val.wqual_type_name(),
                                                      source_position{},
                                                      source_position{});
        res->add_child(L"[functor]", [val] {
            return make_fmt_result<fixed_fmt_result>(L"<empty>",
                                                     std::wstring{},
                                                     source_position{},
                                                     source_position{});
        });
    } else {

        // reading compressed pair containing functor
        auto functor_cp = (*func_obj)["__f_"]["__f_"];

        // reading functor from compressed pair
        value functor;
        if (functor_cp.base_at(0).has_field("__value_")) {
            functor = functor_cp.base_at(0)["__value_"];
        } else {
            functor = functor_cp.base_at(0).base_at(0);
        }

        // formatting functor value
        auto functor_val = ctx().format(functor);
        auto functor_type_def_pos = functor.type().def_pos();

        // if formatted functor contains value position then use it as value pos
        // for entire result
        source_position val_def_pos;
        if (functor_val.val_pos()) {
            val_def_pos = functor_val.val_pos();
        } else {
            val_def_pos = functor_type_def_pos;
        }

        // formatting result value
        std::wostringstream val_str;
        val_str << "[functor = " << functor_val.val() << "]";

        // creating fixed fmt result
        res = std::make_unique<fixed_fmt_result>(val_str.str(),
                                                 val.wqual_type_name(),
                                                 val_def_pos,
                                                 val.type().def_pos());

        res->add_child(L"[functor]", [functor_val]() { return functor_val; });
    }

    // overriding type name in fmt result
    auto disp_type = replace_libcxx_type_name(val);
    auto proxy_res = make_fmt_result<ot_proxy_fmt_result>(make_ref(std::move(res)), disp_type);

    return proxy_res;
}


}
