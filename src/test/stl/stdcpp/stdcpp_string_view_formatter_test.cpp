
/// \file stdcpp_string_view_formatter_test.cpp
/// Contains tests for libstdc++ string_view formatter

#include "../../test2.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>


namespace dbgfmt::test {


namespace {
    enum class sview_kind {
        stdcpp,
        libcxx
    };
}


/// Creates CM for testing string view
static auto build_string_view_cm() {
    cm_builder b;

    b.ns("std")
        .templ("basic_string_view", "T").record("stdcpp_string_view", b.bt_char())
            .ivar("_M_len", b.bt_unsigned_long())
            .ivar("_M_str", b.ptype(b.bt_char()))
        .end().end()

        .ns("__1")
            .templ("basic_string_view", "T").record("libcxx_string_view", b.bt_char())
                .ivar("__data", b.ptype(b.bt_char()))
                .ivar("__size", b.bt_unsigned_long())
            .end().end()
        .end()
    .end();

    return b.build();
}


/// Makes string view with specified pointer to data and size
value make_string_view(sview_kind knd, const value & data, size_t sz) {
    auto sw_type_name = (knd == sview_kind::stdcpp) ? "stdcpp_string_view" : "libcxx_string_view";
    auto size_fld_name = (knd == sview_kind::stdcpp) ? "_M_len" : "__size";
    auto data_fld_name = (knd == sview_kind::stdcpp) ? "_M_str" : "__data";

    auto sview_types = build_string_view_cm();
    auto sview = make_val(sview_types.type(sw_type_name));
    sview[size_fld_name] << sz;
    sview[data_fld_name] << data;
    return sview;
}


BOOST_FIXTURE_TEST_SUITE(std_string_formatter, fmt_test_fixture)


/// Tests displaying empty string view
void test_empty_string_view(context & ctx, sview_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto sview = make_string_view(knd, make_null_to(cm.bt_char()), 0);
    auto res = ctx.format(sview);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"\"\"");
    BOOST_CHECK(res->type() == L"std::basic_string_view<char>");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"0");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(sz->childs_size(), 0);
}

/// Tests displaying empty libstdc++ string view
BOOST_AUTO_TEST_CASE(empty_stdcpp) {
    test_empty_string_view(ctx, sview_kind::stdcpp);
}

/// Tests displaying empty libc++ string view
BOOST_AUTO_TEST_CASE(empty_libcxx) {
    test_empty_string_view(ctx, sview_kind::libcxx);
}


/// Tests displaying simple string view
void test_simple_string_view(context & ctx, sview_kind knd) {
    auto & vctx = test_context::current_context().vctx();
    auto & cm = test_context::current_context().cm();
    auto scm = build_string_view_cm();
    auto str = make_arr(cm.bt_char(), 6);
    str[0] << 'a';
    str[1] << 'b';
    str[2] << 'c';
    str[3] << 'd';
    str[4] << 'e';
    str[5] << 'f';

    auto char_ptr_type = vctx.get_or_create_type(cm.get_or_create_ptr_type(cm.bt_char()));
    auto sview = make_string_view(knd, str.ptr().cast(char_ptr_type) + 2, 3);
    auto res = ctx.format(sview);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"\"cde\"");
    BOOST_CHECK(res->type() == L"std::basic_string_view<char>");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 4);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"3");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(sz->childs_size(), 0);

    auto ch1 = res->child_at(1);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"[0]");
    BOOST_CHECK(ch1->val() == L"'c' (99)");
    BOOST_CHECK(ch1->type() == L"char");
    BOOST_CHECK_EQUAL(ch1->childs_size(), 0);

    auto ch2 = res->child_at(2);
    BOOST_REQUIRE(ch2);
    BOOST_CHECK(ch2->name() == L"[1]");
    BOOST_CHECK(ch2->val() == L"'d' (100)");
    BOOST_CHECK(ch2->type() == L"char");
    BOOST_CHECK_EQUAL(ch2->childs_size(), 0);

    auto ch3 = res->child_at(3);
    BOOST_REQUIRE(ch3);
    BOOST_CHECK(ch3->name() == L"[2]");
    BOOST_CHECK(ch3->val() == L"'e' (101)");
    BOOST_CHECK(ch3->type() == L"char");
    BOOST_CHECK_EQUAL(ch3->childs_size(), 0);
}

/// Tests displaying simple libstdc++ string view
BOOST_AUTO_TEST_CASE(simple_stdcpp) {
    test_simple_string_view(ctx, sview_kind::stdcpp);
}

/// Tests displaying simple libstdc++ string view
BOOST_AUTO_TEST_CASE(simple_libcxx) {
    test_simple_string_view(ctx, sview_kind::libcxx);
}


BOOST_AUTO_TEST_SUITE_END()


}
