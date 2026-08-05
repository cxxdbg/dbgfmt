
/// \file type_impl.hpp
/// Contains definition of the type_impl interface.

#pragma once

#include "source_position.hpp"
#include <memory>
#include <string>


namespace dbgfmt::backend {

class array_type_impl;
class builtin_type_impl;
class enum_type_impl;
class function_type_impl;
class mem_ptr_type_impl;
class pointer_type_impl;
class record_type_impl;
class typedef_type_impl;
class type_context_impl;
class value_context_impl;
class vector_type_impl;


/// Abstract implementation of value type
class type_impl {
public:
    /// Destroys object
    virtual ~type_impl() = default;

    /// Returns pointer to type context impl
    virtual type_context_impl * ctx() const = 0;

    /// Returns name of type
    virtual std::string name() const = 0;

    /// Returns source position of definition of type. Returns invalid
    /// source position if not available
    virtual source_position def_pos() const = 0;

    /// Returns type implementation representing pointer to this type
    virtual const type_impl * pointer_type() const = 0;

    /// Returns size of type in bytes
    virtual size_t size() const = 0;

    /// Tries cast this type to builtin type. Returns null if type is not a builtin type.
    virtual const builtin_type_impl * try_as_builtin() const = 0;

    /// Tries cast this type to array type. Returns null if type is not an array type.
    virtual const array_type_impl * try_as_array() const = 0;

    /// Tries cast this type to vector type. Returns null if type is not a vector type.
    virtual const vector_type_impl * try_as_vector() const = 0;

    /// Tries cast this type to enum type. Returns null if type is not an enum type.
    virtual const enum_type_impl * try_as_enum() const = 0;

    /// Tries cast this type to function type. Returns null if type is not a function type.
    virtual const function_type_impl * try_as_function() const = 0;

    /// Tries cast this type to pointer type. Returns null if type is not a pointer type.
    virtual const pointer_type_impl * try_as_pointer() const = 0;

    /// Tries cast this type to record type. Returns null if type is not a record type.
    virtual const record_type_impl * try_as_record() const = 0;

    /// Tries cast this type to typedef type. Returns null if type is not a typedef type.
    virtual const typedef_type_impl * try_as_typedef() const = 0;

    /// Tries cast this type to pointer to member type. Returns null if type is not pointer to member.
    virtual const mem_ptr_type_impl * try_as_mem_ptr() const = 0;

    /// Returns untypedefed type of this type
    virtual const type_impl * untypedef() const = 0;

    /// Returns untypedefed type of this type
    type_impl * untypedef() {
        return const_cast<type_impl*>(const_cast<const type_impl*>(this)->untypedef());
    }

    /// Returns qualified name of type. Default implementation returns name()
    virtual std::string qual_name() const { return name(); }

    /// Untupedefs this type and tries cast it to specified type.
    /// Returns nullptr if type can't be casted.
    template <typename T>
    auto cast() const {
        return dynamic_cast<const T*>(this->untypedef());
    }

    /// Untupedefs this type and tries cast it to specified type.
    /// Returns nullptr if type can't be casted.
    template <typename T>
    auto cast() {
        return dynamic_cast<T*>(this->untypedef());
    }
};


}


