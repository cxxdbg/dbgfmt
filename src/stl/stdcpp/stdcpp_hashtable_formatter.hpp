
/// \file stdcpp_hashtable_formatter.hpp
/// Contains definition of the stdcpp_hashtable_formatter class.

#pragma once

#include "../../container_formatter.hpp"
#include "../../container_iterator_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ hashtable based containers
class stdcpp_hashtable_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


/// Formatter for libstdc++ hashtable iterator
class stdcpp_hashtable_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns pointer to value pointed by iterator
    value get_value_pointer(const value & it) const override;
};


}


