
/// \file enum_formatter.hpp
/// Contains definition of classes for formatting enum values.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Formatter for enum values
class enum_formatter: public formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override { return v.is_enum(); }

    fmt_result_ref format(const value & val) override;
};


}


