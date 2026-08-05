
/// \file simple_formatter.hpp
/// Contains definition of the simple_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include <sstream>


namespace dbgfmt {


/// Helper class for defining formatters that just translate
/// value to string / hex string.
class simple_formatter: public formatter {
public:
    /// Formats value. Calls write function to obtain the value field.
    /// Uses type name as value of the type field. Uses position of definition of type
    /// of value as value of "type pos" field.
    fmt_result_ref format(const value & val) override {
        std::wostringstream val_str;
        write(val, val_str);

        std::wostringstream val_hex_str;
        hex_write(val, val_hex_str);

        return make_fmt_result<fixed_fmt_result>(val_str.str(),
                                                 val.wqual_type_name(),
                                                 source_position{},
                                                 val.type().def_pos());
    }

private:
    /// Writes value string representation to output stream
    virtual void write(const value & val, std::wostream & str) const = 0;

    /// Writes value string representation in hex mode to output stream.
    /// Default implementation calls write.
    virtual void hex_write(const value & val, std::wostream & str) const {
        write(val, str);
    }
};


}


