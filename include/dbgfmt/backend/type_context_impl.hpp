
/// \file type_context_impl.hpp
/// Contains definition of the type_context_impl class.

#pragma once

#include "builtin_type_impl.hpp"
#include <string>


namespace dbgfmt::backend {

class type_impl;


/// Interface for type context implementation. Contains functions for searching types.
class type_context_impl {
public:
    /// Returns pointer to implementation of value type
    /// with specified type name. Returns null pointer if type
    /// does not exist.
    virtual type_impl * find_type(const std::string & nm) = 0;

    /// Returns builtin type of specified kind
    virtual type_impl * bt_type(builtin_type_impl::kind_t kind) = 0;
};


}
