
/// \file value_impl.hpp
/// Contains definition of value_impl class.

#pragma once

#include "format_error.hpp"
#include "type_impl.hpp"
#include "../data/accessor.hpp"
#include <memory>
#include <string>
#include <sstream>


namespace dbgfmt::backend {

class value_context_impl;
class value_impl;
using value_impl_sp = std::shared_ptr<value_impl>;


/// \class value_impl
/// Represents abstract implementation of value
class value_impl {
public:
    /// Destructor, destroys object
    virtual ~value_impl() {}

    /// Returns pointer to value context implementation
    virtual value_context_impl * ctx() const = 0;

    /// Returns pointer to implementation of value raw type
    /// (may return typedef type)
    virtual const type_impl * raw_type() const = 0;

    /// Returns size of value
    virtual std::size_t size() const = 0;

    /// Returns reference to data storage for value
    virtual data::storage & storage() const = 0;

    /// Returns offset in data storage for value
    virtual uint64_t offset() const = 0;

    /// Returns data endianness for value
    virtual dbgfmt::data::endianness endianness() const = 0;

    /// Returns data accessor for value
    data::accessor data() const {
        return {storage(), endianness(), offset()};
    }


    // Common function used in the dbgfmt::value class. We can't move
    // implementation directly into this class because all value_impl
    // function must be virtual to not depend on c++ ABI.

    /// Returns pointer to implementation of value type removing
    /// all typedef types
    virtual const type_impl * type() const = 0;

    /// Returns true if value is boolean
    virtual bool is_bool() const = 0;

    /// Returns true if value is array
    virtual bool is_array() const = 0;

    /// Returns true if value is vector
    virtual bool is_vector() const = 0;

    /// Returns true if value is signed integer
    virtual bool is_signed() const = 0;

    /// Returns true if value is unsigned integer
    virtual bool is_unsigned() const = 0;

    /// Returns true if value is float
    virtual bool is_float() const = 0;

    /// Returns true if value is pointer
    virtual bool is_pointer() const = 0;

    /// Returns true if value is record
    virtual bool is_record() const = 0;

    /// Returns true if value is wchar
    virtual bool is_wchar() const = 0;

    /// Returns true if value is enum
    virtual bool is_enum() const = 0;

    /// Returns true if value is float complex
    virtual bool is_float_complex() const = 0;

    /// Returns true if value if signed int complex
    virtual bool is_signed_complex() const = 0;

    /// Returns true if value is unsigned int complex
    virtual bool is_unsigned_complex() const = 0;

    /// Returns true if value is pointer to member variable
    virtual bool is_mem_ptr() const = 0;

    /// Returns true if value is nullptr_t value
    virtual bool is_nullptr_t() const = 0;

    /// Returns name of type of value without qualifiers
    /// (const, volatile, restrict)
    virtual std::string type_name() const = 0;

    /// Returns type name without qualifiers as unicode string
    virtual std::wstring wtype_name() const = 0;

    /// Returns name of type with qualifiers
    virtual std::string qual_type_name() const = 0;

    /// Returns qualified type name as unicode string
    virtual std::wstring wqual_type_name() const = 0;

    /// Casts value to specified type
    virtual value_impl_sp cast(const type_impl * type) const = 0;

    /// Returns value containing pointer to this value
    virtual value_impl_sp ptr() const = 0;

    /// Assigns another value to this value. Values must be of same size.
    virtual void assign(const value_impl & val) const = 0;

    /// Reads value as boolean
    virtual bool as_bool() const = 0;

    /// Writes boolean value
    virtual void write_bool(bool v) const = 0;

    /// Reads value as signed integer
    virtual std::int64_t as_int64() const = 0;

    /// Writes signed integer to value
    virtual void write_int64(int64_t val) const = 0;

    /// Reads value as unsigned integer
    virtual std::uint64_t as_uint64() const = 0;

    /// Writes unsigned integer to value
    virtual void write_uint64(uint64_t val) const = 0;

    /// Reads value as long double
    virtual long double as_long_double() const = 0;

    /// Writes floating point to value
    virtual void write_long_double(long double val) const = 0;

    /// Returns number of enum items
    virtual std::size_t enum_items_size() const = 0;

    /// Returns name of enum item with specified index
    virtual std::string enum_item_name(std::size_t index) const = 0;

    /// Returns value of enum item with specified index as signed integer
    virtual std::int64_t enum_item_value(std::size_t index) const = 0;

    /// Returns true if value has address
    virtual bool has_address() const = 0;

    /// Returns address of value
    virtual std::uint64_t address() const = 0;

    /// Returns size of array value
    virtual std::size_t array_size() const = 0;

    /// Returns array item value at specified index
    virtual value_impl_sp array_at(std::size_t i) const = 0;

    /// Returns size of vector value
    virtual std::size_t vector_size() const = 0;

    /// Returns array item value at specified index
    virtual value_impl_sp vector_at(std::size_t i) const = 0;

    /// Returns true if pointer can be dereferenced
    virtual bool can_dereference() const = 0;

    /// Returns dereferenced value
    virtual value_impl_sp dereference(bool resolve_dyn_type) const = 0;

    /// Adds offset to pointer, returns pointer value which points
    /// to corresponding value
    virtual value_impl_sp add(std::int64_t val) const = 0;

    /// Calculates difference between two pointers or returns
    /// error if pointers have different types or types with no size
    virtual std::int64_t difference(const value_impl * v) const = 0;

    /// Returns name of function to which pointer points to
    virtual std::string function_name() const = 0;

    /// Returns offset in function to which pointer points to
    virtual std::uint64_t function_offset() const = 0;

    /// Returns name of symbol to which pointer points to
    virtual std::string symbol_name() const = 0;

    /// Returns offset in symbol to which pointer points to
    virtual std::uint64_t symbol_offset() const = 0;

    /// Returns position of definition of object in source code to
    /// that pointer is points to
    virtual source_position pointee_def_pos() const = 0;


    /// Returns name of template class without template parameters
    virtual std::string template_name() const = 0;

    /// Returns number of direct base classes
    virtual std::size_t bases_size() const = 0;

    /// Returns value of direct base class at specified index
    virtual value_impl_sp base_at(std::size_t i) const = 0;

    /// Returns number of fields
    virtual std::size_t fields_size() const = 0;

    /// Returns name of field with specified index
    virtual std::string field_name_at(std::size_t i) const = 0;

    /// Returns name of field with specified index as unicode string
    virtual std::wstring wfield_name_at(std::size_t i) const = 0;

    /// Returns source position of definition of field with specified index
    virtual source_position field_def_pos_at(size_t i) const = 0;

    /// Returns value of field with specified index
    virtual value_impl_sp field_at(std::size_t i) const = 0;

    /// Returns true if record has field with specified name
    virtual bool has_field(const std::string & nm) const = 0;

    /// Returns value of field with specified name, or null shared
    /// pointer if there is no such field
    virtual value_impl_sp field(const std::string & nm) const = 0;

    /// Returns pointer to member function with specified name or
    /// invalid value if member does not exist
    virtual value_impl_sp mem_fun_ptr(const std::string & nm) const = 0;

    /// Returns vector of names of member functions for record
    virtual std::vector<std::string> mem_funcs() const = 0;


    /// Tries find value of field with specified name.
    /// Returns invalid value if field not found
    virtual value_impl_sp try_all_field(const std::string & nm) const = 0;

    /// Returns value of field with specified name. Throws
    /// dbgfmt::format_error exception if field with specified name not found
    virtual value_impl_sp all_field(const std::string & nm) const = 0;

    /// Returns pointer to value of field with specified name
    virtual value_impl_sp all_field_pointer(const std::string &nm) const = 0;


    /// Returns real part of complex value as long double
    virtual long double real_part_as_long_double() const = 0;

    /// Returns img part of complex value as long double
    virtual long double img_part_as_long_double() const = 0;

    /// Writes long double value to real part of comlex value
    virtual void write_real_part_long_double(long double v) const = 0;

    /// Writes long double value to img part of complex value
    virtual void write_img_part_long_double(long double v) const = 0;

    /// Returns real part of complex value as int64
    virtual int64_t real_part_as_int64() const = 0;

    /// Returns img part of complex value as int64
    virtual int64_t img_part_as_int64() const = 0;

    /// Writes signed value to real part of comlex value
    virtual void write_real_part_int64(int64_t v) const = 0;

    /// Writes signed value to img part of complex value
    virtual void write_img_part_int64(int64_t v) const = 0;

    /// Returns real part of complex value as uint64
    virtual uint64_t real_part_as_uint64() const = 0;

    /// Returns img part of complex value as uint64
    virtual uint64_t img_part_as_uint64() const = 0;

    /// Writes unsigned value to real part of comlex value
    virtual void write_real_part_uint64(uint64_t v) const = 0;

    /// Writes unsigned value to img part of complex value
    virtual void write_img_part_uint64(uint64_t v) const = 0;


    /// Formats and returns name of field for pointer to member variable
    virtual std::string
    mem_var_ptr_field_name_and_def_pos(source_position & def_pos) const = 0;
};


}


