
/// \file cm_builder.hpp
/// Contains definition of the cm_builder class.

#pragma once

#include <cm/builder.hpp>
#include "test_context.hpp"


namespace dbgfmt::test {


/// Helper class dervied from cm::builder for building CM for
/// test context
class cm_builder: public cm::builder {
public:
    /// Constructs builder for specified test context
    cm_builder(test_context & tctx):
        cm::builder(tctx.cm(), &tctx.dbg()) {}

    /// Constructs builder for current test context
    cm_builder():
        cm_builder(test_context::current_context()) {}
};


}


