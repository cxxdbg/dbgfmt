
/// \file value_conv_formatter.hpp
/// Contains definition of the value_conv_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"
#include "dbgfmt/context.hpp"


namespace dbgfmt {


/// Helper class for defining new formatters. Converts value
/// and delegates all work to other formatters passing converted
/// value to formatting context. Then converts formatted value
/// if required.
class value_conv_formatter: public formatter {
public:
    /// Formats value. Calls convert function and passes returned result
    /// to formatting context format function
    fmt_result_ref format(const value & val) override {
        return convert(val, ctx().format(convert(val), ctx().fmt_opts().with_show_raw_data(false)));
    }

private:
    /// Converts value
    virtual value convert(const value & val) const = 0;

    /// Converts formatting result. Default implementation returns original result.
    virtual fmt_result_ref convert(const value & val, const fmt_result_ref & res) const {
        return res;
    }
};


}


