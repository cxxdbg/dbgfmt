
/// \file libcxx_utils.hpp
/// Contains declarations of utility functions for testing libc++ formatters.

#pragma once

#include <cm/qual_type.hpp>
#include <cm/record_type.hpp>
#include "../../test2.hpp"


namespace dbgfmt::test {


/// Creates and returns pointer to std::__1::pair type
cm::record_type * get_libcxx_pair_type(const cm::qual_type & first, const cm::qual_type & second);

/// Creates libc++ pair value from first and second values
value make_libcxx_pair(const value & first, const value & second);

/// Makes and returns pointer to std::__1::__compressed_pair with specified type of first value
cm::record_type * get_libcxx_compressed_pair_type(const cm::qual_type & first);


}
