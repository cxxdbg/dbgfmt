
/// \file value_impl_common.hpp
/// Contains definition of the value_impl_common class.

#pragma once

#include "backend/value_impl.hpp"
#include "source_position.hpp"
#include "format_error.hpp"


namespace dbgfmt {


namespace backend {
class pointer_type_impl;
class record_type_impl;
class value_context_impl;
}


/// Contains implementation of common functions used in the dbgfmt::value
/// that don't depend on specific value implementation.
class value_impl_common: public backend::value_impl {
public:
    /// Constructs value implementation with specified reference to value context
    value_impl_common(backend::value_context_impl & c): ctx_{c} {}

    /// Creates value that points to same data storage as this value,
    /// with additional offset
    backend::value_impl_sp create_offs_value(const backend::type_impl * type,
                                    int64_t offs,
                                    const source_position & def_pos = {}) const {
        return create_value(type, offset() + offs, def_pos);
    }

    /// Creates value that points to same data storage with specified offset from
    /// the beginning of the storage
    virtual backend::value_impl_sp create_value(const backend::type_impl * type,
                                       uint64_t base_offset,
                                       const source_position & def_pos = {}) const = 0;

    /// Returns reference to memory storage
    virtual data::storage & memory() const = 0;

    /// Returns pointer to value context implementation
    backend::value_context_impl * ctx() const override { return &ctx_; }

    /// Returns pointer to implementation of value type removing
    /// all typedef types
    const backend::type_impl * type() const override;

    /// Returns true if value is boolean
    bool is_bool() const override;

    /// Returns true if value is array
    bool is_array() const override;

    /// Returns true if value is vector
    bool is_vector() const override;

    /// Returns true if value is signed integer
    bool is_signed() const override;

    /// Returns true if value is unsigned integer
    bool is_unsigned() const override;

    /// Returns true if value is float
    bool is_float() const override;

    /// Returns true if value is pointer
    bool is_pointer() const override;

    /// Returns true if value is record
    bool is_record() const override;

    /// Returns true if value is wchar
    bool is_wchar() const override;

    /// Returns true if value is enum
    bool is_enum() const override;

    /// Returns true if value is float complex
    bool is_float_complex() const override;

    /// Returns true if value if signed int complex
    bool is_signed_complex() const override;

    /// Returns true if value is unsigned int complex
    bool is_unsigned_complex() const override;

    /// Returns true if value is pointer to member variable
    bool is_mem_ptr() const override;

    /// Returns true if value is nullptr_t value
    bool is_nullptr_t() const override;

    /// Returns name of type of value without qualifiers
    /// (const, volatile, restrict)
    std::string type_name() const override;

    /// Returns type name without qualifiers as unicode string
    std::wstring wtype_name() const override;

    /// Returns name of type with qualifiers
    std::string qual_type_name() const override;

    /// Returns qualified type name as unicode string
    std::wstring wqual_type_name() const override;

    /// Casts value to specified type
    backend::value_impl_sp cast(const backend::type_impl * type) const override;

    /// Returns value containing pointer to this value
    backend::value_impl_sp ptr() const override;

    /// Assigns another value to this value. Values must be of same size.
    void assign(const backend::value_impl & val) const override;

    /// Reads value as boolean
    bool as_bool() const override;

    /// Writes boolean value
    void write_bool(bool v) const override;

    /// Reads value as signed integer
    std::int64_t as_int64() const override;

    /// Writes signed integer to value
    void write_int64(int64_t val) const override;

    /// Reads value as unsigned integer
    std::uint64_t as_uint64() const override;

    /// Writes unsigned integer to value
    void write_uint64(uint64_t val) const override;

    /// Reads value as long double
    long double as_long_double() const override;

    /// Writes floating point to value
    void write_long_double(long double val) const override;

    /// Returns number of enum items
    std::size_t enum_items_size() const override;

    /// Returns name of enum item with specified index
    std::string enum_item_name(std::size_t index) const override;

    /// Returns value of enum item with specified index as signed integer
    std::int64_t enum_item_value(std::size_t index) const override;

    /// Returns true if value has address
    bool has_address() const override;

    /// Returns address of value
    std::uint64_t address() const override;

    /// Returns size of array value
    std::size_t array_size() const override;

    /// Returns array item value at specified index
    backend::value_impl_sp array_at(std::size_t i) const override;

    /// Returns size of vector value
    std::size_t vector_size() const override;

    /// Returns vector item value at specified index
    backend::value_impl_sp vector_at(std::size_t i) const override;

    /// Returns true if pointer can be dereferenced
    bool can_dereference() const override;

    /// Returns dereferenced value
    backend::value_impl_sp dereference(bool resolve_dyn_type) const override;

    /// Adds offset to pointer, returns pointer value which points
    /// to corresponding value
    backend::value_impl_sp add(std::int64_t val) const override;

    /// Calculates difference between two pointers or returns
    /// error if pointers have different types or types with no size
    std::int64_t difference(const backend::value_impl * v) const override;

    /// Returns name of function to which pointer points to
    std::string function_name() const override;

    /// Returns offset in function to which pointer points to
    std::uint64_t function_offset() const override;

    /// Returns name of symbol to which pointer points to
    std::string symbol_name() const override;

    /// Returns offset in symbol to which pointer points to
    std::uint64_t symbol_offset() const override;

    /// Returns position of definition of object in source code to
    /// that pointer is points to
    source_position pointee_def_pos() const override;


    /// Returns name of template class without template parameters
    std::string template_name() const override;

    /// Returns number of direct base classes
    std::size_t bases_size() const override;

    /// Returns value of direct base class at specified index
    backend::value_impl_sp base_at(std::size_t i) const override;

    /// Returns number of fields
    std::size_t fields_size() const override;

    /// Returns name of field with specified index
    std::string field_name_at(std::size_t i) const override;

    /// Returns name of field with specified index as unicode string
    std::wstring wfield_name_at(std::size_t i) const override;

    /// Returns source position of definition of field with specified index
    source_position field_def_pos_at(size_t i) const override;

    /// Returns value of field with specified index
    backend::value_impl_sp field_at(std::size_t i) const override;

    /// Returns true if record has field with specified name
    bool has_field(const std::string & nm) const override;

    /// Returns value of field with specified name, or null shared
    /// pointer if there is no such field
    backend::value_impl_sp field(const std::string & nm) const override;

    /// Returns pointer to member function with specified name or
    /// invalid value if member does not exist
    backend::value_impl_sp mem_fun_ptr(const std::string & nm) const override;

    /// Returns vector of names of member functions for record
    std::vector<std::string> mem_funcs() const override;


    /// Tries find value of field with specified name.
    /// Returns invalid value if field not found
    backend::value_impl_sp try_all_field(const std::string & nm) const override;

    /// Returns value of field with specified name. Throws
    /// dbgfmt::format_error exception if field with specified name not found
    backend::value_impl_sp all_field(const std::string & nm) const override;

    /// Returns pointer to value of field with specified name
    backend::value_impl_sp all_field_pointer(const std::string &nm) const override;


    /// Returns real part of complex value as long double
    long double real_part_as_long_double() const override;

    /// Returns img part of complex value as long double
    long double img_part_as_long_double() const override;

    /// Writes long double value to real part of comlex value
    void write_real_part_long_double(long double v) const override;

    /// Writes long double value to img part of complex value
    void write_img_part_long_double(long double v) const override;

    /// Returns real part of complex value as int64
    int64_t real_part_as_int64() const override;

    /// Returns img part of complex value as int64
    int64_t img_part_as_int64() const override;

    /// Writes signed value to real part of comlex value
    void write_real_part_int64(int64_t v) const override;

    /// Writes signed value to img part of complex value
    void write_img_part_int64(int64_t v) const override;

    /// Returns real part of complex value as uint64
    uint64_t real_part_as_uint64() const override;

    /// Returns img part of complex value as uint64
    uint64_t img_part_as_uint64() const override;

    /// Writes unsigned value to real part of comlex value
    void write_real_part_uint64(uint64_t v) const override;

    /// Writes unsigned value to img part of complex value
    void write_img_part_uint64(uint64_t v) const override;

    /// Formats and returns name of field and def pos for pointer to member variable
    std::string mem_var_ptr_field_name_and_def_pos(source_position & def_pos) const override;

    /// Returns data accessor for value
    data::accessor data() const {
        return data::accessor{storage(), endianness(), offset()};
    }

private:
    /// Returns value type as record. Throws format_error if type is not a record.
    const backend::record_type_impl * type_as_record() const;

    /// Returns value type as pointer. Throws format error if type is not a pointer.
    const backend::pointer_type_impl * type_as_pointer() const;

    /// Creates value for record field with specified type, offset, size,
    /// and definition position
    backend::value_impl_sp create_field_val(const backend::type_impl * type,
                                   uint64_t bit_offset,
                                   uint64_t bit_size,
                                   const source_position & def_pos) const;

    /// Tries detect dyanmic type of object located at specified address. First tries read pointer to
    /// virtual table at the beginning of the object and detect type from its name. If can't detect type
    /// from virtual table then calls dynamic_type_at_addr function from value context.
    const backend::type_impl * dynamic_type_at_addr(uint64_t & addr, const backend::type_impl * static_type, size_t ptr_size) const;

    backend::value_context_impl & ctx_;          ///< Reference to value context implementation
};


}


