
/// \file libcxx_tree_formatter.hpp
/// Contains definition of formatter for libc++ tree containers.

#pragma once

#include "../../container_formatter.hpp"
#include "../../container_iterator_formatter.hpp"
#include "../../value_conv_formatter.hpp"


namespace dbgfmt {


/// Formatter for libc++ tree containers
class libcxx_tree_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

private:
    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns type name that should be displayed for user.
    std::wstring disp_type_name(const value & val) const override;
};


/// Formatter for libc++ tree iterator
class libcxx_tree_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

private:
    /// Returns pointer to value pointed by iterator
    value get_value_pointer(const value & it) const override;
};



/// Formatter for libc++ map iterator
class libcxx_map_iterator_formatter: public value_conv_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

private:
    /// Converts value
    value convert(const value & val) const override;
};



}
