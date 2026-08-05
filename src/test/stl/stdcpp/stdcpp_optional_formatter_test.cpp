
/// \file stdcpp_optional_formatter_test.cpp
/// Contains unit tests for libstdc++ optional formatter

#include "stdcpp_utils.hpp"
#include "../../builtin_value.hpp"
#include "../../cm_builder.hpp"
#include "../../fmt_test_fixture.hpp"
#include "../../pointer_value.hpp"
#include "../../record_value.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Creates CM for testing std::optional
static cm::builder_result create_optional_cm(const cm::qual_type & val_type) {
    cm_builder b;
    return b
        .ns("std")
            .templ("optional", "T").record("optional", val_type)
                .ivar("_M_payload", b.typeref("payload"))
            .end().end()
            .record("payload", "payload")
                .base(b.typeref("xunion"))
                .ivar("_M_engaged", b.bt_bool())
            .end()
            .record("xunion", "xunion")
                .ivar("_M_empty", b.typeref("empty_rec"))
                .ivar("_M_payload", val_type)
            .end()
            .record("empty_rec", "empty_rec")
            .end()
        .end()
    .build();
}


BOOST_FIXTURE_TEST_SUITE(stdcpp_variant_formatter_test, fmt_test_fixture)


/// Tests displaying empty int optional
BOOST_AUTO_TEST_CASE(int_empty) {
    auto ocm = create_optional_cm(cm.bt_int());
    record_value var_rec(ocm.rtype("optional"));
    var_rec["_M_payload"]["_M_engaged"] = bool_value{false};

    auto res = ctx.format(var_rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = empty]");
    BOOST_CHECK(res->type() == L"std::optional<int>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto ch1 = res->child_at(0);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"[value]");
    BOOST_CHECK(ch1->val() == L"empty");
    BOOST_CHECK(ch1->type() == L"");
    BOOST_CHECK_EQUAL(ch1->childs_size(), 0);
}


/// Tests displaying int optional
BOOST_AUTO_TEST_CASE(int_simple) {
    auto ocm = create_optional_cm(cm.bt_int());
    record_value var_rec(ocm.rtype("optional"));
    var_rec["_M_payload"]["_M_engaged"] = bool_value{true};
    var_rec["_M_payload"].as<record_value>().base("xunion")["_M_payload"] = 10;

    auto res = ctx.format(var_rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = 10]");
    BOOST_CHECK(res->type() == L"std::optional<int>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto ch1 = res->child_at(0);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"[value]");
    BOOST_CHECK(ch1->val() == L"10");
    BOOST_CHECK(ch1->type() == L"int");
    BOOST_CHECK_EQUAL(ch1->childs_size(), 0);
}


/// Tests displaying optional containing record
BOOST_AUTO_TEST_CASE(record) {
    cm_builder myrec_builder;
    auto myrec_cmres = myrec_builder
        .record("myrec", "myrec")
            .ivar("x", myrec_builder.bt_int())
            .ivar("y", myrec_builder.bt_int())
        .end()
    .build();

    auto ocm = create_optional_cm(myrec_cmres.rtype("myrec"));
    record_value var_rec(ocm.rtype("optional"));
    var_rec["_M_payload"]["_M_engaged"] = bool_value{true};
    var_rec["_M_payload"].as<record_value>().base("xunion")["_M_payload"]["x"] = 10;
    var_rec["_M_payload"].as<record_value>().base("xunion")["_M_payload"]["y"] = 20;

    auto res = ctx.format(var_rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = { x=10 y=20 }]");
    BOOST_CHECK(res->type() == L"std::optional<myrec>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto ch1 = res->child_at(0);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"[value]");
    BOOST_CHECK(ch1->val() == L"{ x=10 y=20 }");
    BOOST_CHECK(ch1->type() == L"myrec");
    BOOST_CHECK_EQUAL(ch1->childs_size(), 2);
}


BOOST_AUTO_TEST_SUITE_END()


}