
/// \file vector_type_impl.hpp
/// Contains definition of the vector_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


/// Represents vector (SSE/AVX) type
class vector_type_impl: virtual public type_impl {
public:
    /// Returns pointer to element type
    virtual const type_impl * base() const = 0;

    /// Returns size of vector
    virtual size_t vector_size() const = 0;
};


}
