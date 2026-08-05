
/// \file array_type_impl.hpp
/// Contains definition of the array_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


/// Abstract implementation of array type
class array_type_impl: virtual public type_impl {
public:
    /// Returns pointer to implementation of base type
    virtual const type_impl * base() const = 0;

    /// Returns size of array
    virtual size_t array_size() const = 0;
};


}


