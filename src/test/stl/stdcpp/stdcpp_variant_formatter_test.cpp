
/// \file stdcpp_variant_formatter_test.cpp
/// Contains unit tests for libstdc++ variant formatter

#include "stdcpp_utils.hpp"
#include "../../builtin_value.hpp"
#include "../../cm_builder.hpp"
#include "../../fmt_test_fixture.hpp"
#include "../../pointer_value.hpp"
#include "../../record_value.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Creates CM for testing std::any
static cm::builder_result create_variant_cm() {
    cm_builder mb;
    auto myrec_cm = mb
        .record("myrec", "myrec")
            .ivar("x", mb.bt_int())
            .ivar("y", mb.bt_int())
        .end()
    .build();

    cm_builder b;
    return b
        .ns("std")
            .templ("variant", "A", "B").record("variant", b.bt_int(), myrec_cm.rtype("myrec"))
                .base(b.typeref("base1"))
            .end().end()
            .record("base1", "base1")
                .base(b.typeref("base2"))
            .end()
            .record("base2", "base2")
                .base(b.typeref("base3"))
            .end()
            .record("base3", "base3")
                .ivar("_M_u", b.typeref("vunion1"))
                .ivar("_M_index", b.bt_unsigned_char())
            .end()
            .record("vunion1", "vunion1")
                .ivar("_M_first", b.typeref("data_int"))
                .ivar("_M_rest", b.typeref("vunion2"))
            .end()
            .record("vunion2", "vunion2")
                .ivar("_M_first", b.typeref("data_myrec"))
                .ivar("_M_rest", b.typeref("vunion3"))
            .end()
            .record("vunion3", "vunion3")
            .end()
            .record("data_int", "data_int")
                .ivar("_M_storage", b.bt_int())
            .end()
            .record("data_myrec", "data_myrec")
                .ivar("_M_storage", myrec_cm.rtype("myrec"))
            .end()
        .end()
    .build();
}


BOOST_FIXTURE_TEST_SUITE(stdcpp_variant_formatter_test, fmt_test_fixture)


/// Tests displaying variant containing int
BOOST_AUTO_TEST_CASE(variant_int) {
    auto vcm = create_variant_cm();
    record_value var_rec(vcm.rtype("variant"));
    var_rec.base("base1").base("base2").base("base3")["_M_index"] = '\0';
    var_rec.base("base1").base("base2").base("base3")["_M_u"]["_M_first"]["_M_storage"] = 200;

    auto res = ctx.format(var_rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = 200]");
    BOOST_CHECK(res->type() == L"std::variant<int, myrec>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 2);

    auto ch1 = res->child_at(0);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"[index]");
    BOOST_CHECK(ch1->val() == L"0");
    BOOST_CHECK(ch1->type() == L"int");
    BOOST_CHECK_EQUAL(ch1->childs_size(), 0);

    auto ch2 = res->child_at(1);
    BOOST_REQUIRE(ch2);
    BOOST_CHECK(ch2->name() == L"[value]");
    BOOST_CHECK(ch2->val() == L"200");
    BOOST_CHECK(ch2->type() == L"int");
    BOOST_CHECK_EQUAL(ch2->childs_size(), 0);
}


/// Tests displaying variant containing record
BOOST_AUTO_TEST_CASE(variant_rec) {
    auto vcm = create_variant_cm();
    record_value var_rec(vcm.rtype("variant"));
    var_rec.base("base1").base("base2").base("base3")["_M_index"] = '\1';
    var_rec.base("base1").base("base2").base("base3")
        ["_M_u"]["_M_rest"]["_M_first"]["_M_storage"]["x"] = 10;
    var_rec.base("base1").base("base2").base("base3")
        ["_M_u"]["_M_rest"]["_M_first"]["_M_storage"]["y"] = 20;

    auto res = ctx.format(var_rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = { x=10 y=20 }]");
    BOOST_CHECK(res->type() == L"std::variant<int, myrec>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 2);

    auto ch1 = res->child_at(0);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"[index]");
    BOOST_CHECK(ch1->val() == L"1");
    BOOST_CHECK(ch1->type() == L"int");
    BOOST_CHECK_EQUAL(ch1->childs_size(), 0);

    auto ch2 = res->child_at(1);
    BOOST_REQUIRE(ch2);
    BOOST_CHECK(ch2->name() == L"[value]");
    BOOST_CHECK(ch2->val() == L"{ x=10 y=20 }");
    BOOST_CHECK(ch2->type() == L"myrec");
    BOOST_CHECK_EQUAL(ch2->childs_size(), 2);
}


BOOST_AUTO_TEST_SUITE_END()


}