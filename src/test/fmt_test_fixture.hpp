
/// \fiel fmt_test_fixture.hpp
/// Contains definition of the fmt_test_fixture class.

#pragma once

#include "test_context.hpp"
#include "dbgfmt/context.hpp"
#include <cm/builder.hpp>


namespace dbgfmt::test {


/// Common fixture for formatting framework tests
struct fmt_test_fixture {
private:
    test_context tctx{data::endianness::little};

public:
    cm::code_model & cm = tctx.cm();
    cm::debug_info & dbg = tctx.dbg();
    context ctx{tctx.vctx(), tctx.vctx()};
    value_context_impl & vctx = tctx.vctx();

    /// Constructs fixture and sets current value context
    fmt_test_fixture() {
        test_context::set_current_context(&tctx);
        ctx.set_show_raw_data(false);
        ctx.set_min_hex_size(8);
        ctx.set_show_ptr_addr(true);
    }

    /// Sets current value context to null
    ~fmt_test_fixture() {
        test_context::set_current_context(nullptr);
    }
};


}


