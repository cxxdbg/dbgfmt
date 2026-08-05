
/// \file libcxx_string_formatter.hpp
/// Contains definition of the libcxx_string_formatter class.

#pragma once

#include "../std_string_formatter.hpp"


namespace dbgfmt {


/// Formatter for libc++++ std::basic_string class
class libcxx_string_formatter: public std_string_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

protected:
    /// Returns container capacity
    std::size_t capacity(const value & val) const override;

    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns type name that should be displayed for user.
    std::wstring disp_type_name(const value & val) const override;
};


}
