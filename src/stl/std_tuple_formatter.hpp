
/// \file std_tuple_formatter.hpp
/// Contains definition of the std_tuple_formatter class.

#pragma once

#include "../container_formatter.hpp"


namespace dbgfmt {


/// Base class for std tuple formatters. Contains common formatting logic.
class std_tuple_formatter: public container_formatter {
protected:
    /// Formats value
    std::wstring format_value(const value & val) const override;

    bool display_size() const override { return false; }
};


}
