
/// \file libcxx_array_formatter.hpp
/// Contains definition of the stdcpp_array_formatter class.

#pragma once

#include "../../container_formatter.hpp"


namespace dbgfmt {


/// \class libcxx_array_formatter
/// Formatter for libc++ array class.
class libcxx_array_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override;
};


}
