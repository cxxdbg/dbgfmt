
/// \file stdcpp_array_formatter
/// Contains definition of the stdcpp_array_formatter class.

#pragma once

#include "../../container_formatter.hpp"


namespace dbgfmt {


/// \class stdcpp_array_formatter
/// Formatter for libstdc++ array class.
class stdcpp_array_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    virtual bool accepts(const value & v) const;

private:
    /// Returns container size
    virtual std::size_t size(const value & val) const;

    /// Returns value object for container element with specified index
    virtual value element(const value & val, std::size_t index) const;
};


}


