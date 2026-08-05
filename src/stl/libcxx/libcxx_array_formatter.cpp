
/// \file libcxx_array_formatter.cpp
/// Contains implementation of the libcxx_array_formatter class.

#include "libcxx_array_formatter.hpp"
#include "libcxx_ns_utils.hpp"
#include "dbgfmt/format_error.hpp"
#include <cassert>


namespace dbgfmt {


bool libcxx_array_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "array");
}


std::size_t libcxx_array_formatter::size(const value & val) const {
    // detecting size from template parameter because libc++ uses array of different
    // size for empty array

    std::istringstream sz_str{val.type().template_param_name(1)};
    size_t sz = 0;
    if (!(sz_str >> sz)) {
        std::ostringstream msg;
        msg << "can't parse std::array size parameter: '" << val.type().template_param_name(1) << "'";
        throw format_error{msg.str()};
    }

    return sz;
}


value libcxx_array_formatter::element(const value & val, std::size_t index) const {
    assert(index < size(val) && "Invalid element index");
    return val["__elems_"][index];
}


std::wstring libcxx_array_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}


}
