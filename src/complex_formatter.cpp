
/// \file complex_formatter.cpp
/// Contains implementation of classes for formatting complex values.

#include "complex_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "format_complex.hpp"
#include <cmath>
#include <sstream>


namespace dbgfmt {


void float_complex_formatter::write(const value & val, std::wostream & str) const {
    write_float_complex(val.real_part_as_long_double(), val.img_part_as_long_double(), str);
}


void int_complex_formatter::write(const value & val, std::wostream & str) const {
    if (ctx().hex()) {
        auto sz = val.size();
        auto msz = ctx().min_hex_size();
        if (val.is_signed_complex()) {
            write_hex_int_complex(val.real_part_as_int64(), val.img_part_as_int64(), sz, msz, str);
        } else {
            write_hex_int_complex(val.real_part_as_uint64(), val.img_part_as_uint64(), sz, msz, str);
        }
    } else {
        if (val.is_signed_complex()) {
            write_int_complex(val.real_part_as_int64(), val.img_part_as_int64(), str);
        } else {
            write_int_complex(val.real_part_as_uint64(), val.img_part_as_uint64(), str);
        }
    }
}


void int_complex_formatter::hex_write(const value & val, std::wostream & str) const {
    write(val, str);
}


}
