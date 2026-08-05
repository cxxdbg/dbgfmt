
/// \file mem_ptr_type_impl.hpp
/// Contains definition of the mem_ptr_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


class record_type_impl;


/// Represents abstract pointer to member type implementation
class mem_ptr_type_impl: virtual public type_impl {
public:
    /// Returns pointer to type of member
    virtual const type_impl * mem_type() const = 0;

    /// Returns pointer to type of object
    virtual const record_type_impl * obj_type() const = 0;
};


}


