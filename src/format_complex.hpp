
/// \file format_complex.hpp
/// Contains definitions of utility functions for formatting complex values

#pragma once

#include "dbgfmt/value.hpp"
#include "dbgfmt/format_error.hpp"
#include "print.hpp"
#include <ostream>


namespace dbgfmt {


/// Gets sign character for img value and makes it positive if needed
template <typename IntType>
static void get_sign_and_img(wchar_t & sign, IntType & img) {
    if (img < static_cast<IntType>(0)) {
        img = -img;
        sign = L'-';
    } else {
        sign = L'+';
    }
}


/// Writes integer complex value to output stream
template <typename IntType>
void write_int_complex(IntType real, IntType img, std::wostream & str) {
    wchar_t sign = 0;
    get_sign_and_img(sign, img);
    str << real << ' ' << sign << ' ' << img << "i";
}


/// Writes hex representation of integer complex value to output stream
template <typename IntType>
void write_hex_int_complex(IntType real,
                           IntType img,
                           size_t sz,
                           size_t min_hex_sz,
                           std::wostream & str) {
    wchar_t sign = 0;
    get_sign_and_img(sign, img);
    print_hex(str, real, sz / 2, min_hex_sz);
    str << ' ' << sign << ' ';
    print_hex(str, img, sz / 2, min_hex_sz);
    str << "i";
}


/// Writes float complex to output stream
inline void write_float_complex(long double real, long double img, std::wostream & str) {
    auto sign = img < 0 ? '-' : '+';
    str << real << ' ' << sign << ' ' << std::abs(img) << "i";
}


/// Writes complex values to output stream
inline void write_values_complex(const value & real,
                                 const value & img,
                                 bool is_hex,
                                 size_t min_hex_sz,
                                 std::wostream & str) {
    if (real.is_float()) {
        write_float_complex(real.as_long_double(), img.as_long_double(), str);
    } else if (real.is_signed()) {
        if (is_hex) {
            write_hex_int_complex(real.as_int64(), img.as_int64(), real.size() + img.size(), min_hex_sz, str);
        } else {
            write_int_complex(real.as_int64(), img.as_int64(), str);
        }
    } else if (real.is_unsigned()) {
        if (is_hex) {
            write_hex_int_complex(real.as_uint64(), img.as_uint64(), real.size() + img.size(), min_hex_sz, str);
        } else {
            write_int_complex(real.as_uint64(), img.as_uint64(), str);
        }
    } else {
        std::ostringstream msg;
        msg << "don't know how to format complex of type '" << real.type_name() << "'";
        throw format_error{msg.str()};
    }
}


}
