
/// \file stdcpp_optional_formatter.cpp
/// Contains implementation of the stdcpp_optional_formatter class.

#include "stdcpp_optional_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


bool stdcpp_optional_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::optional";
}


fmt_result_ref stdcpp_optional_formatter::format(const value & val) {
    // checking for empty optional
    if (!val["_M_payload"]["_M_engaged"].as_bool()) {
        auto res = std::make_unique<fixed_fmt_result>(L"[value = empty]",
                                                      val.wqual_type_name(),
                                                      source_position{},
                                                      val.type().def_pos());
        res->add_child(L"[value]", []{
            return make_fmt_result<fixed_fmt_result>(L"empty",
                                                     L"",
                                                     source_position{},
                                                     source_position{});
        });
        return make_ref(std::move(res));
    }

    // reading value of optional
    auto opt_val = val["_M_payload"]["_M_payload"];

    // formatting value of optional
    auto opt_fmt_val = ctx().format(opt_val);

    // creating value and hex value for formatting result
    std::wostringstream fmt_val;
    fmt_val << L"[value = " << opt_fmt_val.val() << L"]";

    // if formatted value contains value position then use it as value pos
    // for entire result
    source_position val_def_pos;
    if (opt_fmt_val.val_pos()) {
        val_def_pos = opt_fmt_val.val_pos();
    }

    // creating fixed fmt result
    auto res = std::make_unique<fixed_fmt_result>(fmt_val.str(),
                                                  val.wqual_type_name(),
                                                  val_def_pos,
                                                  val.type().def_pos());
    res->add_child(L"[value]", [opt_fmt_val] { return opt_fmt_val; });
    return make_ref(std::move(res));
}


}
