
/// \file stdcpp_array_formatter.cpp
/// Contains implementation of the stdcpp_array_formatter class.

#include "stdcpp_array_formatter.hpp"
#include <cassert>


namespace dbgfmt {


bool stdcpp_array_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::array";
}


std::size_t stdcpp_array_formatter::size(const value & val) const {
    // if _M_elems field is not an array then size is zero
    if (!val["_M_elems"].is_array())
        return 0;

    return val["_M_elems"].array_size();
}


value stdcpp_array_formatter::element(const value & val, std::size_t index) const {
    assert(index < size(val) && "Invalid element index");
    return val["_M_elems"][index];
}


}
