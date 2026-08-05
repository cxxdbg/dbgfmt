
/// \file value_type.hpp
/// Contains definition of the value_type class.

#pragma once

#include "backend/mem_ptr_type_impl.hpp"
#include "backend/pointer_type_impl.hpp"
#include "backend/record_type_impl.hpp"
#include "backend/type_impl.hpp"
#include <sstream>
#include "source_position.hpp"
#include "format_error.hpp"


namespace dbgfmt {


class value_type_impl_accessor;
class context;


/// Wrapper around pointer to value type implementation
class value_type {
    friend class context;
    friend class value;
    friend class value_type_impl_accessor;

public:
    /// Constructs value type wrapper with specified pointer
    /// to value type implementation
    value_type(const backend::type_impl * i = nullptr):
        impl_{i} {}

    /// Returns true if type is valid
    bool is_valid() const {
        return impl_ != nullptr;
    }

    /// Returns true if type is valid
    explicit operator bool() const {
        return is_valid();
    }

    /// Returns type name
    std::string name() const {
        return impl_->name();
    }

    /// Returns position of type definition in source code
    source_position def_pos() const {
        return impl_->def_pos();
    }

    /// Returns name of type with qualifiers
    std::string qual_name() const {
        return impl_->qual_name();
    }

    /// Returns true if type is builtin type
    bool is_builtin() const { return impl_->try_as_builtin() != nullptr; }

    /// Returns true if type is array type
    bool is_array() const { return impl_->try_as_array() != nullptr; }

    /// Returns true if type is vector type
    bool is_vector() const { return impl_->try_as_vector() != nullptr; }

    /// Returns true if type is enum type
    bool is_enum() const { return impl_->try_as_enum() != nullptr; }

    /// Returns true if type is function type
    bool is_function() const { return impl_->try_as_function() != nullptr; }

    /// Returns true if type is pointer type
    bool is_pointer() const { return impl_->try_as_pointer() != nullptr; }

    /// Returns true if type is record type
    bool is_record() const { return impl_->try_as_record() != nullptr; }

    /// Returns true if type is typedef type
    bool is_typedef() const { return impl_->try_as_typedef() != nullptr; }

    /// Returns true if type is pointer to member type
    bool is_mem_ptr() const { return impl_->try_as_mem_ptr() != nullptr; }

    /// Returns type of pointer to this type
    value_type pointer() const {
        auto pimpl = impl_->pointer_type();
        if (!pimpl) {
            std::ostringstream msg;
            msg << "Can't get pointer type for type '" << impl_->name() << "'";
            throw format_error{msg.str()};
        }
        return value_type{impl_->pointer_type()};
    }

    /// Return type size
    size_t size() const { return impl_->size(); }


    //////////////////////////////////////////////////
    // Records

    /// Returns base type of pointer type
    value_type ptr_base() const {
        return value_type{as_pointer().base()};
    }


    //////////////////////////////////////////////////
    // Records

    /// Returns template name for record
    std::string template_name() const {
        return as_record().template_name();
    }

    /// Returns number of template parameter names in type name
    size_t template_param_names_size() const {
        return as_record().template_param_names_size();
    }

    /// Returns name of template parameter in type name
    std::string template_param_name(size_t idx) const {
        return as_record().template_param_name(idx);
    }

    /// Returns number of template parameters
    size_t template_params_size() const {
        return as_record().template_params_size();
    }

    /// Returns true if specified template parameter is a type
    bool template_param_is_type(size_t index) const {
        return as_record().template_param_is_type(index);
    }

    /// Returns type of template parameter at specified index
    value_type template_param_type(size_t index) const {
        return as_record().template_param_type(index);
    }


    //////////////////////////////////////////////////
    // Pointer to member type functions

    /// Returns member type for pointer to member type
    value_type mem_ptr_mem_type() const { return value_type{as_mem_ptr().mem_type()}; }

    /// Returns objecty type for pointer to member type
    value_type mem_ptr_obj_type() const { return value_type{as_mem_ptr().obj_type()}; }

private:
    /// Returns pointer to type implementation
    auto impl() const { return impl_; }

    /// Returns reference to implementation as builtin type.
    /// Throws format_error if type is not a builtin type.
    const backend::builtin_type_impl & as_builtin() const {
        auto res = impl_->try_as_builtin();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a builtin type";
            throw format_error{msg.str()};
        }

        return *res;
    }

    /// Returns reference to implementation as array type.
    /// Throws format_error if type is not an array type.
    const backend::array_type_impl & as_array() const {
        auto res = impl_->try_as_array();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a array type";
            throw format_error{msg.str()};
        }

        return *res;
    }

    /// Returns reference to implementation as enum type.
    /// Throws format_error if type is not an enum type.
    const backend::enum_type_impl & as_enum() const {
        auto res = impl_->try_as_enum();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a enum type";
            throw format_error{msg.str()};
        }

        return *res;
    }

    /// Returns reference to implementation as function type.
    /// Throws format_error if type is not an function type.
    const backend::function_type_impl & as_function() const {
        auto res = impl_->try_as_function();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a function type";
            throw format_error{msg.str()};
        }

        return *res;
    }

    /// Returns reference to implementation as pointer type.
    /// Throws format_error if type is not a pointer type.
    const backend::pointer_type_impl & as_pointer() const {
        auto res = impl_->try_as_pointer();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a pointer type";
            throw format_error{msg.str()};
        }

        return *res;
    }

    /// Returns reference to implementation as record type.
    /// Throws format_error if type is not a record type.
    const backend::record_type_impl & as_record() const {
        auto res = impl_->try_as_record();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a record type";
            throw format_error{msg.str()};
        }

        return *res;
    }

    /// Returns reference to implementation as typedef type.
    /// Throws format_error if type is not a typedef type.
    const backend::typedef_type_impl & as_typedef() const {
        auto res = impl_->try_as_typedef();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a typedef type";
            throw format_error{msg.str()};
        }

        return *res;
    }

    /// Returns reference to implementation as pointer to member type.
    /// Throws format_error if type is not a pointer to member type.
    const backend::mem_ptr_type_impl & as_mem_ptr() const {
        auto res = impl_->try_as_mem_ptr();
        if (!res) {
            std::ostringstream msg;
            msg << "type '" << name() << "' is not a pointer to member type";
            throw format_error{msg.str()};
        }

        return *res;
    }


    const backend::type_impl * impl_;      ///< Pointer to value type implementation
};


}


