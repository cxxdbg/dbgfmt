
#pragma once

#include "value_type.hpp"
#include "source_position.hpp"
#include "format_error.hpp"
#include "backend/forward.hpp"
#include <string>
#include <vector>


/// \file value.hpp
/// Contains definition of value class.


namespace dbgfmt {


class float_complex_value_impl;
class int_complex_value_impl;


/// \class value
/// Represents value object which holds pointer to abstract value
/// implementation
class value {
public:
    /// Constructor, makes value object with specified pointer to implementation
    value(const backend::const_value_impl_sp &imp = backend::const_value_impl_sp());

    /// Destructor, destroys object
    ~value();

    /// Returns true if value is valid
    bool is_valid() const;

    /// Returns type of value
    value_type type() const;

    /// Returns true if value is boolean
    bool is_bool() const;

    /// Returns true if value is array
    bool is_array() const;

    /// Returns true if value is vector
    bool is_vector() const;

    /// Returns true if value is signed integer
    bool is_signed() const;

    /// Returns true if value is unsigned integer
    bool is_unsigned() const;

    /// Returns true if value is float
    bool is_float() const;

    /// Returns true if value is pointer
    bool is_pointer() const;

    /// Returns true if value is record
    bool is_record() const;

    /// Returns true if value is wchar
    bool is_wchar() const;

    /// Returns true if value is enum
    bool is_enum() const;

    /// Returns true if value is error
    bool is_error() const;

    /// Returns true if value is float complex
    bool is_float_complex() const;

    /// Returns true if value if signed int complex
    bool is_signed_complex() const;

    /// Returns true if value is unsigned int complex
    bool is_unsigned_complex() const;

    /// Returns true if value is pointer to member varialbe
    bool is_mem_ptr() const;

    /// Returns true if value is nullptr_t
    bool is_nullptr_t() const;

    /// Returns name of type of value without qualifiers
    /// (const, volatile, restrict)
    std::string type_name() const;

    /// Returns type name without qualifiers as unicode string
    std::wstring wtype_name() const;

    /// Returns name of type with qualifiers
    std::string qual_type_name() const;

    /// Returns qualified type name as unicode string
    std::wstring wqual_type_name() const;

    /// Returns size of value
    std::size_t size() const;

    /// Casts value to specified type
    value cast(const value_type & type) const;

    /// Returns value that contains pointer to this value
    value ptr() const;

    /// Assigns another value to this value
    void assign(const value & val) const;

    /// Assigns another value to this value
    const value & operator<<(const value & val) const;

    /// Reads value as boolean
    bool as_bool() const;

    /// Writes boolean to value
    void write_bool(bool v) const;

    /// Writes boolean to value
    const value & operator<<(bool v) const;

    /// Reads value as signed integer
    std::int64_t as_int64() const;

    /// Wrtes signed integer to value
    void write_int64(int64_t v) const;

    /// Writes char to value
    const value & operator<<(char v) const;

    /// Writes short to value
    const value & operator<<(short v) const;

    /// Writes int to value
    const value & operator<<(int v) const;

    /// Writes long to value
    const value & operator<<(long v) const;

    /// Writes long long to value
    const value & operator<<(long long v) const;

    /// Reads value as unsigned integer
    std::uint64_t as_uint64() const;

    /// Writes unsigned integer to value
    void write_uint64(uint64_t v) const;

    /// Writes unsigned char to value
    const value & operator<<(unsigned char v) const;

    /// Writes unsigned short to value
    const value & operator<<(unsigned short v) const;

    /// Writes unsigned int to value
    const value & operator<<(unsigned int v) const;

    /// Writes unsigned long to value
    const value & operator<<(unsigned long v) const;

    /// Writes unsigned long long to value
    const value & operator<<(unsigned long long v) const;

    /// Reads value as long double
    long double as_long_double() const;

    /// Writes long double to value
    void write_long_double(long double v) const;

    /// Writes float to value
    const value & operator<<(float v) const;

    /// Writes double to value
    const value & operator<<(double v) const;

    /// Writes long double to value
    const value & operator<<(long double v) const;

    /// Returns number of enum items
    std::size_t enum_items_size() const;

    /// Returns name of enum item with specified index
    std::string enum_item_name(std::size_t index) const;

    /// Returns value of enum item with specified index as signed integer
    std::int64_t enum_item_value(std::size_t index) const;

    /// Returns true if value has address
    bool has_address() const;

    /// Returns address of value
    std::uint64_t address() const;

    /// Returns size of array value
    std::size_t array_size() const;

    /// Returns array item value at specified index
    value array_at(std::size_t i) const;

    /// Returns size of vector value
    std::size_t vector_size() const;

    /// Returns array item value at specified index
    value vector_at(std::size_t i) const;

    /// Returns array or vector item value at specified index
    const value operator[](std::size_t i) const;

    /// Returns true if pointer can be dereferenced
    bool can_dereference() const;

    /// Returns dereferenced value
    value dereference(bool resolve_dyn_type = true) const;

    /// Returns dereferenced value with resolved dynamic type.
    const value operator*() const;

    /// Adds offset to pointer, returns pointer value which points
    /// to corresponding value
    value add(std::int64_t val) const;

    /// Adds offset to pointer, returns pointer value which points
    /// to corresponding value
    const value operator+(std::int64_t val) const;

    /// Substracts offset from pointer
    const value operator-(std::int64_t val) const;

    /// Calculates difference between two pointers or returns
    /// error if pointers have different types or types with no size
    std::int64_t difference(const value & v) const;

    /// Calculates difference between two pointers or returns
    /// error if pointers have different types or types with no size
    std::int64_t operator-(const value & v) const;

    /// Returns name of function to which pointer points to
    std::string function_name() const;

    /// Returns function name as unicode string
    std::wstring wfunction_name() const;

    /// Returns offset in function to which pointer points to
    std::uint64_t function_offset() const;

    /// Returns name of symbol to which pointer points to
    std::string symbol_name() const;

    /// Returns symbol name as unicode string
    std::wstring wsymbol_name() const;

    /// Returns offset in symbol to which pointer points to
    std::uint64_t symbol_offset() const;

    /// Returns position of definition of object in source code to
    /// that pointer is points to
    source_position pointee_def_pos() const;


    /// Returns name of template class without template parameters
    std::string template_name() const;

    /// Returns number of template parameters
    std::size_t template_params_size() const;

    /// Returns template parameter with specified index
    std::string template_param(std::size_t index) const;

    /// Returns number of direct base classes
    std::size_t bases_size() const;

    /// Returns value of direct base class at specified index
    value base_at(std::size_t i) const;

    /// Returns number of fields
    std::size_t fields_size() const;

    /// Returns name of field with specified index
    std::string field_name_at(std::size_t i) const;

    /// Returns name of field with specified index as unicode string
    std::wstring wfield_name_at(std::size_t i) const;

    /// Returns source position of definition of field with specified index
    source_position field_def_pos_at(size_t i) const;

    /// Returns value of field with specified index
    value field_at(std::size_t i) const;

    /// Returns pointer to field with specified index
    value field_pointer_at(std::size_t i) const;

    /// Returns true if record has field with specified name
    bool has_field(const std::string & nm) const;

    /// Returns value of field with specified name, or null shared
    /// pointer if there is no such field
    value field(const std::string & nm) const;

    /// Returns pointer to member function with specified name or
    /// invalid value if member does not exist
    value mem_fun_ptr(const std::string & nm) const;

    /// Returns vector of names of member functions for record
    std::vector<std::string> mem_funcs() const;


    /// Tries find value of field with specified name.
    /// Returns invalid value if field not found
    value try_all_field(const std::string & nm) const;

    /// Returns value of field with specified name. Throws
    /// dbgfmt::format_error exception if field with specified name not found
    value all_field(const std::string & nm) const;

    /// Returns pointer to value of field with specified name
    value all_field_pointer(const std::string &nm) const;

    /// Returns value of field with specified name, or null shared
    /// pointer if there is no such field (including base classes)
    const value operator[](const std::string & nm) const;


    /// Returns error message
    std::wstring err_msg() const;


    /// Returns real part of complex value as long double
    long double real_part_as_long_double() const;

    /// Returns img part of complex value as long double
    long double img_part_as_long_double() const;

    /// Writes long double value to real part of comlex value
    void write_real_part_long_double(long double v) const;

    /// Writes long double value to img part of complex value
    void write_img_part_long_double(long double v) const;

    /// Returns real part of complex value as int64
    int64_t real_part_as_int64() const;

    /// Returns img part of complex value as int64
    int64_t img_part_as_int64() const;

    /// Writes signed value to real part of comlex value
    void write_real_part_int64(int64_t v) const;

    /// Writes signed value to img part of complex value
    void write_img_part_int64(int64_t v) const;

    /// Returns real part of complex value as uint64
    uint64_t real_part_as_uint64() const;

    /// Returns img part of complex value as uint64
    uint64_t img_part_as_uint64() const;

    /// Writes unsigned value to real part of comlex value
    void write_real_part_uint64(uint64_t v) const;

    /// Writes unsigned value to img part of complex value
    void write_img_part_uint64(uint64_t v) const;


    /// Formats and returns name of field and def pos for pointer to member variable
    std::string mem_var_ptr_field_name_and_def_pos(source_position & def_pos) const;

private:
    backend::const_value_impl_sp impl_;          ///< Pointer to implementation
};


}


