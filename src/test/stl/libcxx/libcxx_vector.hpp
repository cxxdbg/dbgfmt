
/// \file libcxx_vector.hpp
/// Contains declaration of functions for creating types and values for libc++ vector.

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test::tcm::libcxx::vector {
    /// Gets or creates type of libc++ vector
    cm::type_t * type(const cm::qual_type & elt_type);

    /// Creates value of libc++ vector from begin/end pointers
    value make(const cm::qual_type & elt_type, const value & begin, const value & end);

    /// Creates value of libc++ vector from vector of values
    value make(const cm::qual_type & elt_type, const std::vector<value> & vals);
}
