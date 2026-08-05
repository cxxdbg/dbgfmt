
/// \file stdcpp_string_formatter.cpp
/// Contains implementation of the libstdc++ string formatter.

#include "stdcpp_string_formatter.hpp"
#include "dbgfmt/context.hpp"
#include <sstream>


namespace dbgfmt {


bool stdcpp_string_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::basic_string";
}


std::size_t stdcpp_string_formatter::capacity(const value &val) const {
    return static_cast<std::size_t>(get_rep(val)["_M_capacity"].as_uint64());
}


std::size_t stdcpp_string_formatter::size(const value & val) const {
    return static_cast<std::size_t>(get_rep(val)["_M_length"].as_uint64());
}


value stdcpp_string_formatter::element(const value & val, std::size_t index) const {
    return *(val["_M_dataplus"]["_M_p"] + index);
}


value stdcpp_string_formatter::get_rep(const value & val) const {
    auto rep_type = ctx().find_type(val.type_name() + "::_Rep");
    return *(val["_M_dataplus"]["_M_p"].cast(rep_type.pointer()) - 1);
}


}
