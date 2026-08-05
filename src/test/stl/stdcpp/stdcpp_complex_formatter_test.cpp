
/// \file stdcpp_complex_formatter_test.cpp
/// Contains unit tests for libstdc++ std::complex formatter

#include "../../test2.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(stdcpp_complex_formatter_test, fmt_test_fixture)


/// Tests formatting simple complex record with complex builtin value
BOOST_AUTO_TEST_CASE(format_simple_complex_builtin) {
    auto std = cm.get_or_create_namespace("std");
    auto rtype = std->create_named_record("complex");
    rtype->create_field("_M_value", cm.bt_complex_float());
    dbg.make_def_rec_layout(rtype);

    auto rec = make_val(rtype);
    rec["_M_value"].write_real_part_long_double(11.1f);
    rec["_M_value"].write_img_part_long_double(-10.2f);
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"11.1 - 10.2i");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting simple complex record with real and img parts
BOOST_AUTO_TEST_CASE(format_simple_complex_pair) {
    cm_builder b;
    auto ctype = b
        .ns("std")
            .templ("complex", "T").record("complex", b.bt_short())
                .ivar("_M_real", b.bt_short())
                .ivar("_M_imag", b.bt_short())
            .end().end()
        .end()
    .build().type("complex");

    auto c = make_val(ctype);
    c["_M_real"] << 11;
    c["_M_imag"] << -10;
    auto res = ctx.format(c);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"11 - 10i");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting simple complex record with real and img parts in hex mode
BOOST_AUTO_TEST_CASE(format_simple_complex_pair_hex) {
    cm_builder b;
    auto ctype = b
        .ns("std")
            .templ("complex", "T").record("complex", b.bt_short())
                .ivar("_M_real", b.bt_short())
                .ivar("_M_imag", b.bt_short())
            .end().end()
        .end()
    .build().type("complex");

    auto c = make_val(ctype);
    c["_M_real"] << 11;
    c["_M_imag"] << -10;
    ctx.set_hex(true);
    auto res = ctx.format(c);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x000B - 0x000Ai");
    BOOST_CHECK(res->childs_size() == 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
