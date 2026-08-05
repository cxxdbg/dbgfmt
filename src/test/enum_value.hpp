
/// \file enum_value.hpp
/// Contains definition of the enum_value class.

#pragma once

#include "value.hpp"


namespace dbgfmt::test {


/// Enum value builder
class enum_value: public value<cm::enum_type> {
public:
    /// Constructs enum value builder
    enum_value(test_context & ctx,
               const cm::qual_type_t<cm::enum_type> & type,
               int val):
    value(ctx, type) {
        data().write_signed(val, size());
    }

    /// Constructs enum value builder
    enum_value(const cm::qual_type_t<cm::enum_type> & type, int val):
        enum_value(test_context::current_context(), type, val) {}
};


}


