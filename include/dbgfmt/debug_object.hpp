/// \file debug_object.hpp
/// Contains definition of the debug_object class.

#pragma once

#include "value_type.hpp"
#include "source_position.hpp"
#include "format_error.hpp"
#include "backend/debug_object_impl.hpp"


namespace dbgfmt {

class function;


/// Represents debug object in program
class debug_object {
public:
    /// Constructs debug object with specified pointer implementation
    explicit debug_object(const backend::debug_object_impl * impl):
        impl_{impl} {}

    /// Returns true if object is valid
    bool is_valid() const { return impl_ != nullptr; }

    /// Returns true if object is valid
    explicit operator bool() const { return is_valid(); }

    /// Returns object address
    uint64_t addr() const { return impl().addr(); }

    /// Returns object name
    std::string name() const { return impl().name(); }

    /// Returns object type
    value_type type() const { return value_type{impl().type()}; }

    /// Returns position of definition of object
    source_position def_pos() const { return impl().def_pos(); }

    /// Returns true if debug object is a function
    bool is_function() const { return impl_->as_function() != nullptr; }

    /// Converts this debug object to function. Throws exception
    /// if object is not a function
    inline function as_function() const;

private:
    /// Checks that object is valid. Returns reference to implementation.
    /// Throws exception if object is invalid.
    const backend::debug_object_impl & impl() const {
        if (!is_valid()) {
            throw format_error{"access to invalid debug object"};
        }

        return *impl_;
    }

    const backend::debug_object_impl * impl_;            ///< Pointer to implementation
};


/// Represents function in program
class function: public debug_object {
public:
    /// Constructs function object with specified pointer to implementation
    function(const backend::function_debug_object_impl * func):
        debug_object{func}, func_{func} {}

    /// Returns type of base record for member function or invalid type
    /// if function is not a member function.
    value_type try_record_type() const {
        return value_type{impl().record_type()};
    }

    /// Returns type of base record for member function. Throws format_error
    /// if function is not a member function
    value_type record_type() const {
        auto res = try_record_type();
        if (res) {
            return res;
        }

        std::ostringstream msg;
        msg << "function '" << name() << "' is not a member function";
        throw format_error{msg.str()};
    }
    
private:
    /// Checks that object is valid. Returns reference to implementation.
    /// Throws exception if object is invalid.
    const backend::function_debug_object_impl & impl() const {
        if (!is_valid()) {
            throw format_error{"access to invalid function object"};
        }

        return *func_;
    }

    const backend::function_debug_object_impl * func_;   ///< Pointer to implementation 
};


inline function debug_object::as_function() const {
    function res{impl().as_function()};
    if (res.is_valid()) {
        return res;
    }

    std::ostringstream msg;
    msg << "object '" << name() << "' at address " << addr() << " is not a function";
    throw dbgfmt::format_error(msg.str());
}


}
