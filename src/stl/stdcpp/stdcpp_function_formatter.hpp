
/// \file stdcpp_function_formatter.hpp
/// Contains definition of formatter for libstdc++ function

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Formatter fot libstdc++ function
class stdcpp_function_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;

private:
    /// Checks if functor is stored inline in function storage
    bool is_inline_functor(const value_type & functor_type);
};


}


