
/// \file float_formatter.hpp
/// Contains definitions of classes for formatting float values.

#pragma once

#include "simple_formatter.hpp"


namespace dbgfmt {


/// Formatter for float values
class float_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override { return v.is_float(); }

private:
    /// Writes value to output stream
    void write(const value & val, std::wostream & str) const override {
        str << val.as_long_double();
    }
};


}


