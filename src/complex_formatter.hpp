
/// \file complex_formatter.hpp
/// Contains definitions of classes for formatting complex values.

#pragma once

#include "simple_formatter.hpp"


namespace dbgfmt {


/// Float complex values formatter
class float_complex_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override {
        return v.is_float_complex();
    }

private:
    /// Writes string representation of value to output stream
    void write(const value & val, std::wostream & str) const override;
};


/// Integer complex values formatter
class int_complex_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override {
        return v.is_signed_complex() || v.is_unsigned_complex();
    }

private:
    /// Writes string representation of value to output stream
    void write(const value & val, std::wostream & str) const override;

    /// Writes hex string representation of value to output stream
    void hex_write(const value & val, std::wostream & str) const override;
};


}


