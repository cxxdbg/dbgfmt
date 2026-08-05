
/// \file stdcpp_variant_formatter.hpp
/// Contains definition of the stdcpp_variant_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Formatter fot libstdc++ variant
class stdcpp_variant_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;
};


}
