
/// \file libcxx_complex_formatter_test.cpp
/// Contains unit tests for libc++ std::complex formatter

#include "../../test2.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(libcxx_complex_formatter_test, fmt_test_fixture)


/// Tests formatting simple complex record
BOOST_AUTO_TEST_CASE(format_simple) {
    cm_builder b;
    auto ctype = b
        .ns("std").ns("__1")
            .templ("complex", "T").record("complex", b.bt_float())
                .ivar("__re_", b.bt_float())
                .ivar("__im_", b.bt_float())
            .end().end()
        .end().end()
    .build().type("complex");

    auto c = make_val(ctype);
    c["__re_"] << 11.1f;
    c["__im_"] << -10.2f;
    auto res = ctx.format(c);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"11.1 - 10.2i");
    BOOST_CHECK(res->childs_size() == 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
