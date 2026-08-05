
/// \file error_value.hpp
/// Contains definition of the error_value class.

#pragma once

#include "value.hpp"


namespace dbgfmt::test {


/// Builder for error values
class error_value: public value<> {
public:
    /// Constructs builder of error value of specified type
    error_value(test_context & ctx, const cm::qual_type & type):
        value(0, ctx, type) {}

    /// Constructs builder of error value of specified type
    error_value(const cm::qual_type & type):
        error_value(test_context::current_context(), type) {}
};


}


