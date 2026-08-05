
/// \file stdcpp_valarray_formater.hpp
/// Contains definition of the stdcpp_valarray_formatter class.

#pragma once

#include "../../container_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ valarray
class stdcpp_valarray_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value. May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns valarray size
    size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


/// Formatter for libstdc++ slice
class stdcpp_slice_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value. May throw format_error exception.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;
};


/// Formatter for libstdc++ slice_array
class stdcpp_slice_array_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value. May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns slice_array size
    size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


}


