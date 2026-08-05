
/// \file int_formatter.hpp
/// Contains definitions of classes for formatting int values.

#pragma once

#include "simple_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "print.hpp"


namespace dbgfmt {


/// Formatter for signed int values
class int_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override {
        return v.is_signed() && v.size() != 1;
    }

private:
    /// Writes value to output stream
    void write(const value & val, std::wostream & str) const override {
        if (ctx().hex()) {
            print_hex(str, val.as_int64(), val.size(), ctx().min_hex_size());
        } else {
            str << val.as_int64();
        }
    }

    /// Writes hex value to output stream
    void hex_write(const value & val, std::wostream & str) const override {
        write(val, str);
    }
};


/// Formatter for unsigned int values
class uint_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override {
        return v.is_unsigned() && v.size() != 1;
    }

private:
    /// Writes value to output stream
    void write(const value & val, std::wostream & str) const override {
        if (ctx().hex()) {
            print_hex(str, val.as_uint64(), val.size(), ctx().min_hex_size());
        } else {
            str << val.as_uint64();
        }
    }

    /// Writes hex value to output stream
    void hex_write(const value & val, std::wostream & str) const override {
        write(val, str);
    }
};


}


