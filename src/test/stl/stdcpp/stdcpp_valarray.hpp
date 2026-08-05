
/// \file stdcpp_valarray.hpp
/// Contains declarations of utility functions for testing libstdc++ valarray
/// and related classes formatters

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test {


/// Gets or creates type of std::valarray with specified type of element
cm::record_type * get_stdcpp_valarray_type(const cm::qual_type & elt_type);

/// Gets or creates type of std::slice_array with specified type of element
cm::record_type * get_stdcpp_slice_array_type(const cm::qual_type & elt_type);

/// Gets or creates type of std::slice
cm::record_type * get_stdcpp_slice_type();

/// Creates valarray from specified vector of values
value make_stdcpp_valarray(const cm::qual_type & elt_type, const std::vector<value> & elts);

/// Creates libstdc++ slice with specified offset, size, and stride
value make_stdcpp_slice(size_t offs, size_t size, size_t stride);

/// Creates libstdc++ slice_array with specified element type, array of values, size, and stride
value make_stdcpp_slice_array(const cm::qual_type & elt_type,
                              const std::vector<value> & elts,
                              size_t size,
                              size_t stride);


}


