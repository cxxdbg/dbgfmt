
/// \file debug_object_impl.hpp
/// Contains definition of the debug_object_impl class.

#pragma once

#include "type_impl.hpp"
#include <string>


namespace dbgfmt::backend {

class function_debug_object_impl;
class record_type_impl;


/// Interface for accessing debug info of object located in memory
class debug_object_impl {
public:
    /// Returns address of object
    virtual uint64_t addr() const = 0;

    /// Returns name of object
    virtual std::string name() const = 0;

    /// Returns type of object
    virtual type_impl * type() const = 0;

    /// Returns position of definition of object
    virtual source_position def_pos() const = 0;

    /// Casts this object to function. Returns null pointer if object is not a function.
    virtual const function_debug_object_impl * as_function() const = 0;
};


/// Interface for accessing debug info of function
class function_debug_object_impl: virtual public debug_object_impl {
public:
    /// Returns type of record for member function or null if function
    /// is not a member or parent record can't be found
    virtual const record_type_impl * record_type() const = 0;
};


}


