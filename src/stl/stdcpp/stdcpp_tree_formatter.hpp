
/// \file stdcpp_tree_formatter.hpp
/// Contains definition of the stdcpp_tree_formatter class.

#pragma once

#include "../../container_formatter.hpp"
#include "../../container_iterator_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ tree containers
class stdcpp_tree_formatter: public container_formatter {
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


/// Formatter for libstdc++ tree iterator
class stdcpp_tree_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    virtual bool accepts(const value & v) const;

private:
    /// Returns pointer to value pointed by iterator
    virtual value get_value_pointer(const value & it) const;
};



}


