
/// \file libcxx_string.hpp
/// Contains definition of functions for creating libc++ string types and values.

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test {


/// Creates code model for testing libc++ std::string. Returns CM builder result
cm::builder_result libcxx_string_cm(const cm::qual_type & elt_type, bool alternate_layout = false);

/// Creates libc++ string
value create_libcxx_string(const cm::qual_type & char_type,
                           const std::vector<value> & chars,
                           bool alternate_layout = false);


}
