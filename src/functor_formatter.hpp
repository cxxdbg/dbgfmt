
/// \file functor_formatter.hpp
/// Contains definition of the functor_formatter class.

#pragma once

#include "record_formatter.hpp"


namespace dbgfmt {


/// Formatter for records which formats them as user defined functors
class functor_formatter: public record_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;

    /// Returns pointer to operator() member function or invalid value if not found
    static value get_call_operator(const value & v);
};


}
