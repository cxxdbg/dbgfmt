
/// \file mem_var_formatter.hpp
/// Contains definition of the mem_var_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Formatter for pointer to member variables
class mem_var_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return v.type().is_mem_ptr() && !v.type().mem_ptr_mem_type().is_function();
    }

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;
};


}


