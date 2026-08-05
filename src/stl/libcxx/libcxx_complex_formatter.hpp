
/// \file libcxx_complex_formatter.hpp
/// Contains definition of libc++ std::complex formatter.

#pragma once

#include "../../simple_formatter.hpp"
#include "../../format_complex.hpp"
#include "libcxx_ns_utils.hpp"


namespace dbgfmt {


/// Formatter for libc++ std::complex
class libcxx_complex_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "complex");
    }

private:
    /// Writes value string representation to output stream
    void write(const value & val, std::wostream & str) const override {
        write_values_complex(val["__re_"], val["__im_"], false, ctx().min_hex_size(), str);
    }

    /// Writes value string representation in hex mode to output stream
    void hex_write(const value & val, std::wostream & str) const override {
        write_values_complex(val["__re_"], val["__im_"], true, ctx().min_hex_size(), str);
    }
};


}
