
/// \file libcxx_hash_table_formatter.hpp
/// Contains definition of formatters for libc++ hash table containers and iterators.

#pragma once

#include "../../container_formatter.hpp"
#include "../../container_iterator_formatter.hpp"
#include "../../value_conv_formatter.hpp"


namespace dbgfmt {


/// Formatter for libc++ hash table based containers
class libcxx_hash_table_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns container size
    size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns type name that should be displayed for user.
    std::wstring disp_type_name(const value & val) const override;
};


/// Formatter for libc++ hash table iterator
class libcxx_hash_table_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns pointer to value pointed by iterator
    value get_value_pointer(const value & it) const override;
};


/// Formatter for libc++ hash map iterator
class libcxx_hash_map_iterator_formatter: public value_conv_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Converts value
    value convert(const value & val) const override;
};


}
