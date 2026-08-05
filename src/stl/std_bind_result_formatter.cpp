
/// \file std_bind_result_formatter.cpp
/// Contains implementation of the std_bind_result_formatter class.

#include "std_bind_result_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "../utils.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


fmt_result_ref std_bind_result_formatter::format(const value & val) {

    std::wostringstream str;

    str << "std::bind(";

    // formatting functor name

    auto target = bind_target(val);

    source_position val_def_pos;
    format_functor(str, val_def_pos, target);

    // formatting parameters
    auto args = bound_arguments(val);
    auto formatted_args = ctx().format(args, ctx().fmt_opts().with_show_raw_data(false));
    for (size_t i = 0, e = formatted_args.childs_size(); i < e; ++i) {
        auto arg = formatted_args.child_at(i);
        str << ", " << arg.val();
    }


    str << ")";

    auto res = std::make_unique<fixed_fmt_result>(str.str(),
                                                  val.wqual_type_name(),
                                                  val_def_pos,
                                                  val.type().def_pos());

    res->add_child(L"[target]", [this, target] {
        return ctx().format(target);
    });

    res->add_child(L"[args]", [this, args] {
        return ctx().format(args);
    });

    return make_ref(std::move(res));
}


void std_bind_result_formatter::format_functor(std::wostream & str,
                                               source_position & val_pos,
                                               const value & val) const {

    auto new_fmt_opts = ctx().fmt_opts()
                            .with_show_ptr_addr(false)
                            .with_show_raw_data(false)
                            .with_rec_as_functor(true);
    auto fmt_res = ctx().format(val, new_fmt_opts);

    str << fmt_res->val();
    val_pos = fmt_res.val_pos();
}


}
