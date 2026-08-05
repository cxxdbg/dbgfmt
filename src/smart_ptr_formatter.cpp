
/// \file smart_ptr_formatter.cpp
/// Contains implementation of the smart_ptr_formatter class.

#include "smart_ptr_formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/context.hpp"
#include "proxy_fmt_result.hpp"
#include <cassert>


namespace dbgfmt {


fmt_result_ref smart_ptr_formatter::format(const value & val) {
    auto ptr = get_pointer(val);

    // formatting pointer
    auto fptr = ctx().format(ptr);

    // creating formatting result
    auto res = std::make_unique<fixed_fmt_result>(fptr.val(),
                                                  val.wqual_type_name(),
                                                  fptr.val_pos(),
                                                  val.type().def_pos());

    // adding extra childs
    for (size_t i = 0, e = extra_childs_size(); i < e; ++i) {
        res->add_child(extra_child_name(i), [this, val, i] {
            return extra_child(val, i);
        });
    }

    // adding [ptr] child
    res->add_child(L"[ptr]", [fptr] { return fptr; });

    auto res_ref = make_ref(std::move(res));
    return make_fmt_result<ot_proxy_fmt_result>(res_ref, disp_type_name(val));
}


std::size_t smart_ptr_formatter::extra_childs_size() const {
    return 0;
}


std::wstring smart_ptr_formatter::extra_child_name(size_t idx) const {
    assert(false && "smart_ptr_formatter::extra_child_name should not be called");
    return {};
}


fmt_result_ref smart_ptr_formatter::extra_child(const value & v,
                                                std::size_t index) const {
    assert(false && "smart_ptr_formatter::extra_child should not be called");
    return {};
}


std::wstring smart_ptr_formatter::disp_type_name(const value & v) const {
    return v.wqual_type_name();
}


}
