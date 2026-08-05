
/// \file stdcpp_tuple_formatter_test.cpp
/// Contains unit tests for libstdc++ tuple formatter

#include "libcxx/libcxx_tuple.hpp"
#include "stdcpp/stdcpp_tuple.hpp"
#include "../test2.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {

namespace {
    enum tuple_kind {
        stdcpp,
        libcxx
    };
}


/// Makes tuple of specified kind
template <typename ... Values>
static value make_tuple(tuple_kind knd, Values && ... vals) {
    if (knd == tuple_kind::stdcpp) {
        return make_stdcpp_tuple(std::forward<Values>(vals)...);
    } else {
        return make_libcxx_tuple(std::forward<Values>(vals)...);
    }
}


BOOST_FIXTURE_TEST_SUITE(std_tuple_formatter_test, fmt_test_fixture)


/// Tests formatting empty tuple
static void test_empty_tuple(context & ctx, tuple_kind knd) {
    auto tp = make_tuple(knd);
    auto res = ctx.format(tp);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"<>");
    BOOST_CHECK(res->type() == L"std::tuple<>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 0);
}

/// Tests formatting empty libstdc++ tuple
BOOST_AUTO_TEST_CASE(empty_stdcpp_tuple) {
    test_empty_tuple(ctx, tuple_kind::stdcpp);
}

/// Tests formatting empty libc++ tuple
BOOST_AUTO_TEST_CASE(empty_libcxx_tuple) {
    test_empty_tuple(ctx, tuple_kind::libcxx);
}


/// Tests formatting simple tuple
void test_simple_tuple(context & ctx, tuple_kind knd) {
    auto tp = make_tuple(knd, make_int(10), make_int(20));
    auto res = ctx.format(tp);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"<10, 20>");
    BOOST_CHECK(res->type() == L"std::tuple<int, int>");
    BOOST_REQUIRE(res->childs_size() == 2);

    auto c1 = res->child_at(0);
    BOOST_CHECK(c1->name() == L"[0]");
    BOOST_CHECK(c1->val() == L"10");
    BOOST_CHECK(c1->type() == L"int");
    BOOST_CHECK(c1->childs_size() == 0);

    auto c2 = res->child_at(1);
    BOOST_CHECK(c2->name() == L"[1]");
    BOOST_CHECK(c2->val() == L"20");
    BOOST_CHECK(c2->type() == L"int");
    BOOST_CHECK(c2->childs_size() == 0);
}

/// Tests formatting simple libstdc++ tuple
BOOST_AUTO_TEST_CASE(simple_stdcpp_tuple) {
    test_simple_tuple(ctx, tuple_kind::stdcpp);
}

/// Tests formatting simple libc++ tuple
BOOST_AUTO_TEST_CASE(simple_libcxx_tuple) {
    test_simple_tuple(ctx, tuple_kind::libcxx);
}


/// Tests formatting tuple with empty element
void test_empty_element_tuple(context & ctx, tuple_kind knd) {
    auto & tctx = test_context::current_context();
    auto rtype = tctx.cm().create_named_record("my_rec");
    tctx.dbg().make_def_rec_layout(rtype);
    auto tp = make_tuple(knd, make_val(rtype), make_int(20));
    auto res = ctx.format(tp);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"<{  }, 20>");
    BOOST_CHECK(res->type() == L"std::tuple<my_rec, int>");
    BOOST_REQUIRE(res->childs_size() == 2);

    auto c1 = res->child_at(0);
    BOOST_CHECK(c1->name() == L"[0]");
    BOOST_CHECK(c1->val() == L"{  }");
    BOOST_CHECK(c1->type() == L"my_rec");
    BOOST_CHECK(c1->childs_size() == 0);

    auto c2 = res->child_at(1);
    BOOST_CHECK(c2->name() == L"[1]");
    BOOST_CHECK(c2->val() == L"20");
    BOOST_CHECK(c2->type() == L"int");
    BOOST_CHECK(c2->childs_size() == 0);
}

/// Tests formatting libstdc++ tuple with empty element
BOOST_AUTO_TEST_CASE(empty_element_stdcpp_tuple) {
    test_empty_element_tuple(ctx, tuple_kind::stdcpp);
}

/// Tests formatting libc++ tuple with empty element
BOOST_AUTO_TEST_CASE(empty_element_libcxx_tuple) {
    test_empty_element_tuple(ctx, tuple_kind::libcxx);
}


BOOST_AUTO_TEST_SUITE_END()


}
