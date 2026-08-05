
/// \file nullptr_formatter.hpp
/// Contains definition of the nullptr_formatter class.

#pragma once

#include "simple_formatter.hpp"


namespace dbgfmt {


/// Formatter for nullptr_t type
class nullptr_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override {
        return v.is_nullptr_t();
    }

private:
    /// Writes value to output stream
    void write(const value & val, std::wostream & str) const override {
        ctx().write_address(str, val.as_uint64());
    }

    /// Writes hex value to output stream
    void hex_write(const value & val, std::wostream & str) const override {
        ctx().write_address(str, val.as_uint64());
    }
};


};
