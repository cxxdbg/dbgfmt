
/// \file stdcpp_complex_formatter.hpp
/// Contains definition of the stdcpp_complex_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ std::complex type
class stdcpp_complex_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

    /// Formats value
    fmt_result_ref format(const value & val) override;
};


}


