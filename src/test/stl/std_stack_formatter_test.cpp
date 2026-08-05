
#include "libcxx/libcxx_deque.hpp"
#include "stdcpp/stdcpp_deque.hpp"
#include "../cm_builder.hpp"
#include "../fmt_test_fixture.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Creates CM for testing libstdc++ stack
static cm::builder_result stdcpp_stack_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std")
            .templ("stack", "T").record("stack", elt_type)
                .ivar("c", get_stdcpp_deque_type(elt_type))
            .end().end()
        .end()
    .build();
}


/// Creates libstdc++ stack from vector of values
static value make_stdcpp_stack(const cm::qual_type & elt_type,
                               const std::vector<value> & vals,
                               std::size_t start) {
    auto rec = make_val(stdcpp_stack_cm(elt_type).rtype("stack"));
    rec["c"] << make_stdcpp_deque(elt_type, vals, start);
    return rec;
}


/// Creates CM for testing libc++ stack
static cm::builder_result libcxx_stack_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("stack", "T").record("stack", elt_type)
                .ivar("c", get_libcxx_deque_type(elt_type))
            .end().end()
        .end().end()
    .build();
}


/// Creates libc++ stack from vector of values
static value make_libcxx_stack(const cm::qual_type & elt_type,
                               const std::vector<value> & vals,
                               std::size_t start) {
    auto rec = make_val(libcxx_stack_cm(elt_type).rtype("stack"));
    rec["c"] << make_libcxx_deque(elt_type, vals, start);
    return rec;
}


BOOST_FIXTURE_TEST_SUITE(std_stack_formatter, fmt_test_fixture)


void check_empty_stack(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 0]");
    BOOST_CHECK(res->type() == L"std::stack<int>");

    BOOST_REQUIRE(res->childs_size() == 1);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"0");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(empty_stdcpp_stack) {
    auto res = ctx.format(make_stdcpp_stack(cm.bt_int(), {}, 10));
    check_empty_stack(res);
}

BOOST_AUTO_TEST_CASE(empty_libcxx_stack) {
    auto res = ctx.format(make_libcxx_stack(cm.bt_int(), {}, 10));
    check_empty_stack(res);
}


void check_simple_stack(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 1]");
    BOOST_CHECK(res->type() == L"std::stack<int>");

    BOOST_REQUIRE(res->childs_size() == 2);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"1");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto item0 = res->child_at(1);
    BOOST_REQUIRE(item0);
    BOOST_CHECK(item0->name() == L"[0]");
    BOOST_CHECK(item0->val() == L"10");
    BOOST_CHECK(item0->type() == L"int");
    BOOST_CHECK(item0->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(simple_stdcpp_stack) {
    auto stack = make_stdcpp_stack(cm.bt_int(), {make_int(10)}, 1);
    auto res = ctx.format(stack);
    check_simple_stack(res);
}

BOOST_AUTO_TEST_CASE(simple_libcxx_stack) {
    auto stack = make_libcxx_stack(cm.bt_int(), {make_int(10)}, 1);
    auto res = ctx.format(stack);
    check_simple_stack(res);
}


void check_multinode_stack(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 2]");
    BOOST_CHECK(res->type() == L"std::stack<int>");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"2");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto item0 = res->child_at(1);
    BOOST_REQUIRE(item0);
    BOOST_CHECK(item0->name() == L"[0]");
    BOOST_CHECK(item0->val() == L"10");
    BOOST_CHECK(item0->type() == L"int");
    BOOST_CHECK(item0->childs_size() == 0);

    auto item1 = res->child_at(2);
    BOOST_REQUIRE(item1);
    BOOST_CHECK(item1->name() == L"[1]");
    BOOST_CHECK(item1->val() == L"20");
    BOOST_CHECK(item1->type() == L"int");
    BOOST_CHECK(item1->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(multinode_stdcpp_stack) {
    std::vector<value> vals;
    vals.push_back(make_int(10));
    vals.push_back(make_int(20));

    auto stack = make_stdcpp_stack(cm.bt_int(), vals, 127);
    auto res = ctx.format(stack);
    check_multinode_stack(res);
}

BOOST_AUTO_TEST_CASE(multinode_libcxx_stack) {
    std::vector<value> vals;
    vals.push_back(make_int(10));
    vals.push_back(make_int(20));

    auto stack = make_libcxx_stack(cm.bt_int(), vals, 2000);
    auto res = ctx.format(stack);
    check_multinode_stack(res);
}


BOOST_AUTO_TEST_SUITE_END()


}
