
/// \file record_type_impl.hpp
/// Contains definition of the record_type_impl class.

#pragma once

#include "type_impl.hpp"
#include <vector>


namespace dbgfmt::backend {


class value_impl;
using value_impl_sp = std::shared_ptr<value_impl>;


/// Abstract record type implementation
class record_type_impl: virtual public type_impl {
public:
    /// Returns name of template class without template parameters
    virtual std::string template_name() const = 0;

    /// Returns number of template parameter names
    virtual size_t template_param_names_size() const = 0;

    /// Returns name of template parameter with specified index
    virtual std::string template_param_name(size_t index) const = 0;

    /// Returns number of template parameters
    virtual size_t template_params_size() const = 0;

    /// Returns true if template parameter is a type
    virtual bool template_param_is_type(size_t index) const = 0;

    /// Returns template parameter type
    virtual const type_impl * template_param_type(size_t index) const = 0;

    /// Returns true if record is dynamic type (has virtual methods and vtable)
    virtual bool is_dynamic() const = 0;


    /// Returns number of direct base classes
    virtual size_t bases_size() const = 0;

    /// Returns type of direct base class at specified index
    virtual const record_type_impl * base_type_at(size_t i) const = 0;

    /// Returns offset of base class value at specified index for object located at specifeid address
    virtual uint64_t base_offset_at(size_t i, uint64_t obj_addr) const = 0;


    /// Returns number of fields
    virtual size_t fields_size() const = 0;

    /// Returns name of field with specified index
    virtual std::string field_name_at(size_t i) const = 0;

    /// Returns source position of definition of record field at
    /// specified index. Returns invalid source position if not avaliable.
    virtual source_position field_def_pos_at(size_t i) const = 0;

    /// Returns type of field with specified index
    virtual type_impl * field_type_at(size_t i) const = 0;

    /// Returns offset of field in bits with specified index
    virtual uint64_t field_offset_at(size_t i) const = 0;

    /// Returns size in bits of field with specified index
    virtual uint64_t field_size_at(size_t i) const = 0;

    /// Gets type, offset in bits, size in bits, and definition position
    /// of field with specified name. Returns nullptr if field was not found
    virtual const type_impl * find_field(const std::string & nm,
                                         uint64_t * offset,
                                         uint64_t * size,
                                         source_position * pos,
                                         bool check_bases = true) const = 0;

    /// Rerurns type of member function with specified name, or 0 if function
    /// with such name not found
    virtual type_impl * mem_fun_type(const std::string & nm) const = 0;

    /// Returns address of member function with specified name, or 0 if
    /// address of function not found
    virtual uint64_t mem_fun_addr(const std::string & nm) const = 0;

    /// Returns vector of names of member functions of record
    virtual std::vector<std::string> mem_funcs() const = 0;
};


}


