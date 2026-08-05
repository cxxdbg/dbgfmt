
/// \file stdcpp_cxx11_string_formatter.cpp
/// Contains implementation of the stdcpp_cxx11_string_formatter class.

#include "stdcpp_cxx11_string_formatter.hpp"


namespace dbgfmt {


bool stdcpp_cxx11_string_formatter::accepts(const value & v) const {
    if (!v.is_record())
        return false;

    return v.template_name() == "std::__cxx11::basic_string";
}


std::size_t stdcpp_cxx11_string_formatter::capacity(const value &val) const {
    if (size(val) < 16) {
        // local buffer is used
        return 15;
    }

    return static_cast<std::size_t>(val["_M_allocated_capacity"].as_uint64());
}


std::size_t stdcpp_cxx11_string_formatter::size(const value & val) const {
    return static_cast<std::size_t>(val["_M_string_length"].as_uint64());
}


value stdcpp_cxx11_string_formatter::element(const value & val, std::size_t index) const {
    return *(val["_M_dataplus"]["_M_p"] + index);
}


}
