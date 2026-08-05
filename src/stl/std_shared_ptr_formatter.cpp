
/// \file std_shared_ptr_formatter.cpp
/// Contains implementation of the std_shared_ptr_formatter class.

#include "std_shared_ptr_formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include <cassert>
#include <sstream>


namespace dbgfmt {


std::size_t std_shared_ptr_formatter::extra_childs_size() const {
    return 2;
}


std::wstring std_shared_ptr_formatter::extra_child_name(size_t idx) const {
    switch (idx) {
    case 0:
        return L"[use count]";
    case 1:
        return L"[weak count]";
    default:
        assert(false && "Invalid child index");
        return {};
    }
}


fmt_result_ref std_shared_ptr_formatter::extra_child(const value & v, std::size_t index) const {
    assert(index < 2 && "invalid child index");

    std::wstring val;

    uint64_t uval = 0;
    if (index == 0) {
        uval = get_use_count(v);
    } else {
        uval = get_weak_count(v);
    }

    std::wostringstream val_str;
    val_str << uval;
    val = val_str.str();

    return make_fmt_result<fixed_fmt_result>(val,
                                             L"int",
                                             source_position{},
                                             source_position{});
}


}
