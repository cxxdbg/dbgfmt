
/// \file libcxx_valarray_formatter.hpp
/// Contains definition of foratter classes for libc++ valarray and related classes.

#pragma once

#include "../../container_formatter.hpp"


namespace dbgfmt {


/// Formatter for libc++ valarray
class libcxx_valarray_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value. May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns valarray size
    size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override;
};


/// Formatter for libc++ slice
class libcxx_slice_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value. May throw format_error exception.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;
};


/// Formatter for libc++ slice_array
class libcxx_slice_array_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value. May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns slice_array size
    size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override;
};


}
