
/// \file type_impl_common.hpp
/// Contains definition of the type_impl_common class.

#pragma once

#include "backend/type_impl.hpp"


namespace dbgfmt {


/// Contains implementations of common functions of type_impl class
/// that don't depend on specific implementation
class type_impl_common: virtual public backend::type_impl {
public:
    /// Casts this type to builtin type. Returns null if type is not a builtin type.
    const backend::builtin_type_impl * try_as_builtin() const override;

    /// Casts this type to array type. Returns null if type is not an array type.
    const backend::array_type_impl * try_as_array() const override;

    /// Casts this type to vector type. Returns null if type is not a vector type.
    const backend::vector_type_impl * try_as_vector() const override;

    /// Casts this type to enum type. Returns null if type is not an enum type.
    const backend::enum_type_impl * try_as_enum() const override;

    /// Casts this type to function type. Returns null if type is not an enum type.
    const backend::function_type_impl * try_as_function() const override;

    /// Casts this type to pointer type. Returns null if type is not a pointer type.
    const backend::pointer_type_impl * try_as_pointer() const override;

    /// Casts this type to record type. Returns null if type is not a record type.
    const backend::record_type_impl * try_as_record() const override;

    /// Casts this type to typedef type. Returns null if type is not a typedef type.
    const backend::typedef_type_impl * try_as_typedef() const override;

    /// Casts this type to pointer to member type. Returns null if type is not pointer to member.
    const backend::mem_ptr_type_impl * try_as_mem_ptr() const override;

    /// Returns untypedefed type of this type
    const backend::type_impl * untypedef() const override;
};


}


