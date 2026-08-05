
/// \file stdcpp_variant_formatter.cpp
/// Contains implementation of the stdcpp_variant_formatter class.

#include "stdcpp_variant_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


bool stdcpp_variant_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::variant";
}


fmt_result_ref stdcpp_variant_formatter::format(const value & val) {
    // reading index of value
    auto idx = val["_M_index"].as_uint64();

    // getting first variadic union value
    auto u_val = val["_M_u"];

    // iterating over variading union to required index
    for(uint64_t i = 0; i < idx; ++i) {
        u_val = u_val["_M_rest"];
    }

    // reading value of variant
    auto var_val = u_val["_M_first"]["_M_storage"];

    // formatting value of variant
    auto fmt_var_val = ctx().format(var_val);

    // creating value and hex value for formatting result
    std::wostringstream fmt_val;
    fmt_val << L"[value = " << fmt_var_val.val() << L"]";

    // if formatted value contains value position then use it as value pos
    // for entire result
    source_position val_def_pos;
    if (fmt_var_val.val_pos()) {
        val_def_pos = fmt_var_val.val_pos();
    }

    // creating fixed fmt result
    auto res = std::make_unique<fixed_fmt_result>(fmt_val.str(),
                                                  val.wqual_type_name(),
                                                  val_def_pos,
                                                  val.type().def_pos());
    res->add_child(L"[index]", [idx] {
        std::wostringstream str;
        str << idx;
        return make_fmt_result<fixed_fmt_result>(str.str(),
                                                 L"int",
                                                 source_position{},
                                                 source_position{});
    });
    res->add_child(L"[value]", [fmt_var_val] { return fmt_var_val; });
    return make_ref(std::move(res));
}


}
