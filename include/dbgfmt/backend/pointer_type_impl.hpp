
/// \file pointer_type_impl.hpp
/// Contains definition of the pointer_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


/// Represents abstract pointer type implementation
class pointer_type_impl: virtual public type_impl {
public:
    /// Returns true if pointer type is reference
    virtual bool is_reference() const = 0;

    /// Returns pointer to base type implementation
    virtual const type_impl * base() const = 0;

    /// Returns true if pointer can be dereferenced
    virtual bool can_dereference() const = 0;
};


}


