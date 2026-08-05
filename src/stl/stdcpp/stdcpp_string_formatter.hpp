
/// \file stdcpp_String_formatter.hpp
/// Contains definition of the stdc++ string formatter

#pragma once

#include "../std_string_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ string class
class stdcpp_string_formatter: public std_string_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    virtual bool accepts(const value & v) const;

protected:
    /// Returns container capacity. Should be called only if
    /// has_capacity returned true. Default implementation calls assert(false).
    virtual std::size_t capacity(const value & val) const;

    /// Returns container size
    virtual std::size_t size(const value & val) const;

    /// Returns value object for container element with specified index
    virtual value element(const value & val, std::size_t index) const;

    /// Returns _Rep record for string record value
    value get_rep(const value & val) const;
};


}


