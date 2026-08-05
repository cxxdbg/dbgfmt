
/// \file stdcpp_string.hpp
/// Contains definitions of function for creating libstdc++ string types and values.

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test {


/// Gets or creates type for libstdc++ std::string
cm::record_type * get_stdcxx_string_type(const cm::qual_type & elt_type);

/// Creates string record with from specified vector of characters
value create_stdcxx_string(const cm::qual_type & char_type, const std::vector<value> & chars);

/// Gets or creates std::__cxx11::basic_string type
cm::record_type * get_stdcxx_cxx11_basic_string_type(const cm::qual_type & char_type);

/// Creates C++11 string value
value create_stdcxx_cxx11_string(const cm::qual_type & char_type, const std::vector<value> & chars);

}
