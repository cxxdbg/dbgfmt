
/// \file lambda_formatter.hpp
/// Contains definition of the lambda_formatter class.

#pragma once

#include "functor_formatter.hpp"


namespace dbgfmt {


/// Formatter for lambda functions
class lambda_formatter: public functor_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;
};


}


