
/// \file stdcpp_stack_formatter.hpp
/// Contains definition of the libstdc++ stack formatter.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// libstdc++ stack formatter
class stdcpp_stack_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;
};


}


