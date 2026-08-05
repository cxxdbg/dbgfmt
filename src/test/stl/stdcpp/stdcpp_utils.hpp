
/// \file stdcpp_utils.hpp
/// Contains definitions of utility functions for stdc++ formatters tests.

#pragma once

#include <cm/qual_type.hpp>
#include <cm/record_type.hpp>


namespace dbgfmt::test {


/// Gets or creates type for instantiation of the libstdc++
/// __gnu_cxx::__aligned_membuf template for specified data type
cm::record_type * get_stdcpp_aligned_membuf_type(const cm::qual_type & type);


/// Gets or creates type for instantiation of the libstdc++
/// std::aligned_storage with specified size
cm::record_type * get_stdcpp_aligned_storage_type(size_t sz);


/// Gets or creates type for instantiation of the libstdc++
/// __gnu_cxx::algined_buffer template for specified data type
cm::record_type * get_stdcpp_aligned_buffer_type(const cm::qual_type & type);


/// Gets or creates type for std::pair
cm::record_type * get_stdcpp_pair_type(const cm::qual_type & first,
                                       const cm::qual_type & second);


}


