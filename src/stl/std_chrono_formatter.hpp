
/// \file std_chrono_formatter.hpp
/// Contains definition of formatters for std::chrono types

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Formatter for the std::chrono::duration type
class std_chrono_duration_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;
};


/// Formatter for the std::chrono::time_point type
class std_chrono_time_point_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;
};


}
