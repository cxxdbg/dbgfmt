
/// \file bool_formatter.hpp
/// Contains definition of classes for formatting bool values.

#pragma once

#include "simple_formatter.hpp"
#include "print.hpp"


namespace dbgfmt {


/// Formatter for bool values
class bool_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override { return v.is_bool(); }

private:
    /// Formats value
    void write(const value & val, std::wostream & str) const override {
        auto uval = val.as_uint64();
        if (uval <= 1) {
            write_bool(val, str);
            return;
        }

        // corrupted bool value
        if (ctx().hex()) {
            print_hex(str, uval, val.size(), ctx().min_hex_size());
        } else {
            str << uval;
        }
    }

    /// Formats hex value
    void hex_write(const value & val, std::wostream & str) const override {
        write(val, str);
    }

    // Writes normal boolean value
    void write_bool(const value & val, std::wostream & str) const {
        str << (val.as_bool() ? L"true" : L"false");
    }
};


}


