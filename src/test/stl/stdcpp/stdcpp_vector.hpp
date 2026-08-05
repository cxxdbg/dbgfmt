
/// \file stdcpp_vector.hpp
/// Contains definitions of functions for building libstdc++ vector types and values.

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test {


/// Creates code model for testing std::vector. Returns CM builder result
cm::builder_result stdcpp_vector_cm(const cm::qual_type & elt_type);

/// Gets or creates libstdc++ vector type for specified element type
cm::type_t * get_stdcpp_vector_type(const cm::qual_type & elt_type);

/// Makes libstc++ vector with specified pointers to begin and end
value make_stdcpp_vector_begin_end(const cm::qual_type & elt_type, const value & begin, const value & end);

/// Creates libstdc++ vector record from specified vector of values
value make_stdcpp_vector(const cm::qual_type & elt_type, const std::vector<value> & vals);


}
