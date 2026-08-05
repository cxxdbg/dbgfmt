
/// \file array_formatter.hpp
/// Contains definition of the array_formatter class.

#pragma once

#include "container_formatter.hpp"


namespace dbgfmt {


/// Formatter for plain array values
class array_formatter: public container_formatter {
public:
    /// Should never be called, always returns false
    bool accepts(const value & v) const override { return v.is_array(); }

    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


}


