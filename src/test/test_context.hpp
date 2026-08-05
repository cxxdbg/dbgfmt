
/// \file test_context.hpp
/// Contains definition of the test_context.

#pragma once

#include "value_context_impl.hpp"
#include "dbgfmt/data/accessor.hpp"
#include "dbgfmt/data/endianness.hpp"
#include <map>


namespace dbgfmt::test {


/// Testing context. Stores info required for formatters testing
/// (code models, debug info, fake memory)
class test_context {
public:
    /// Constructs test context with specified endianness
    test_context(data::endianness end):
        vctx_{cm_, dbg_, end} {}

    /// Returns reference to code model
    auto & cm() { return cm_; }

    /// Returns reference to debug info
    auto & dbg() { return dbg_; }

    /// Returns reference to value context
    auto & vctx() { return vctx_; }

    /// Returns data endianness
    auto endianness() const { return vctx_.endianness(); }

    /// Returns reference to auto memory for value allocation
    auto & memory() { return vctx_.memory(); }

    /// Allocates and returns data for specified type
    data::accessor alloc_data(const cm::const_qual_type & type) {
        return vctx_.alloc_data(type);
    }

    /// Allocates and returns error data for specified type
    data::accessor alloc_error_data(const cm::const_qual_type & type) {
        return vctx_.alloc_error_data(type);
    }

    /// Returns reference to current value context
    static auto & current_context() {
        assert(current_context_ != nullptr && "current value context was not set");
        return *current_context_;
    }

    /// Sets current value context
    static void set_current_context(test_context * ctx) {
        current_context_ = ctx;
    }

private:
    /// CM code model
    cm::code_model cm_;

    /// CM debug info
    cm::debug_info dbg_{cm_};

    /// value context
    value_context_impl vctx_;

    /// Data endianness
    data::endianness end_;

    /// Memory for value allocation
    auto_memory memory_;

    /// Pointer to current value context
    static test_context * current_context_;
};


}


