
/// \file stdcpp_valarray_formatter_test.cpp
/// Contains unit tests for libstdc++ valarray and related classes formatters

#include "libcxx/libcxx_valarray.hpp"
#include "stdcpp/stdcpp_valarray.hpp"
#include "../test2.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(std_valarray_formatter_test, fmt_test_fixture)


void check_valarray_empty(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"0");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(sz->childs_size(), 0);
}

/// Tests displaying empty libstdc++ valarray
BOOST_AUTO_TEST_CASE(stdcpp_valarray_empty) {
    auto v = make_stdcpp_valarray(cm.bt_int(), {});
    auto res = ctx.format(v);
    check_valarray_empty(res);
}

/// Tests displaying empty libc++ valarray
BOOST_AUTO_TEST_CASE(libcxx_valarray_empty) {
    auto v = make_libcxx_valarray(cm.bt_int(), {});
    auto res = ctx.format(v);
    check_valarray_empty(res);
}



void check_valarray_simple(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_REQUIRE_EQUAL(res->childs_size(), 3);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"2");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(sz->childs_size(), 0);

    auto val1 = res->child_at(1);
    BOOST_REQUIRE(val1);
    BOOST_CHECK(val1->name() == L"[0]");
    BOOST_CHECK(val1->val() == L"10");
    BOOST_CHECK(val1->type() == L"int");
    BOOST_CHECK_EQUAL(val1->childs_size(), 0);

    auto val2 = res->child_at(2);
    BOOST_REQUIRE(val2);
    BOOST_CHECK(val2->name() == L"[1]");
    BOOST_CHECK(val2->val() == L"20");
    BOOST_CHECK(val2->type() == L"int");
    BOOST_CHECK_EQUAL(val2->childs_size(), 0);
}

/// Tests displaying simple libstdc++ valarray
BOOST_AUTO_TEST_CASE(stdcpp_valarray_simple) {
    auto v = make_stdcpp_valarray(cm.bt_int(), {make_int(10), make_int(20)});
    auto res = ctx.format(v);
    check_valarray_simple(res);
}

/// Tests displaying simple libc++ valarray
BOOST_AUTO_TEST_CASE(libcxx_valarray_simple) {
    auto v = make_libcxx_valarray(cm.bt_int(), {make_int(10), make_int(20)});
    auto res = ctx.format(v);
    check_valarray_simple(res);
}


void check_slice(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[offset = 10, size = 20, stride = 30]");
    BOOST_CHECK(res->type() == L"std::slice");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 3);

    auto offs_child = res->child_at(0);
    BOOST_REQUIRE(offs_child);
    BOOST_CHECK(offs_child->name() == L"[offset]");
    BOOST_CHECK(offs_child->val() == L"10");
    BOOST_CHECK(offs_child->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(offs_child->childs_size(), 0);

    auto size_child = res->child_at(1);
    BOOST_REQUIRE(size_child);
    BOOST_CHECK(size_child->name() == L"[size]");
    BOOST_CHECK(size_child->val() == L"20");
    BOOST_CHECK(size_child->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(size_child->childs_size(), 0);

    auto stride_child = res->child_at(2);
    BOOST_REQUIRE(stride_child);
    BOOST_CHECK(stride_child->name() == L"[stride]");
    BOOST_CHECK(stride_child->val() == L"30");
    BOOST_CHECK(stride_child->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(stride_child->childs_size(), 0);
}

/// Tests displaying libstdc++ slice class
BOOST_AUTO_TEST_CASE(stdcpp_slice) {
    auto slice = make_stdcpp_slice(10, 20, 30);
    auto res = ctx.format(slice);
    check_slice(res);
}

/// Tests displaying libc++ slice class
BOOST_AUTO_TEST_CASE(libcxx_slice) {
    auto slice = make_libcxx_slice(10, 20, 30);
    auto res = ctx.format(slice);
    check_slice(res);
}


void check_slice_array_empty(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[size = 0]");
    BOOST_CHECK(res->type() == L"std::slice_array<int>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto sz_child = res->child_at(0);
    BOOST_REQUIRE(sz_child);
    BOOST_CHECK(sz_child->name() == L"[size]");
    BOOST_CHECK(sz_child->val() == L"0");
    BOOST_CHECK(sz_child->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(sz_child->childs_size(), 0);
}

/// Tests displaying empty libstdc++ slice_array
BOOST_AUTO_TEST_CASE(stdcpp_slice_array_empty) {
    std::vector<value> vals = {
        make_int(10),
        make_int(20),
        make_int(30),
        make_int(40)
    };
    auto sarr = make_stdcpp_slice_array(cm.bt_int(), vals, 0, 2);
    auto res = ctx.format(sarr);
    check_slice_array_empty(res);
}

/// Tests displaying empty libc++ slice_array
BOOST_AUTO_TEST_CASE(libcxx_slice_array_empty) {
    std::vector<value> vals = {
        make_int(10),
        make_int(20),
        make_int(30),
        make_int(40)
    };
    auto sarr = make_libcxx_slice_array(cm.bt_int(), vals, 0, 2);
    auto res = ctx.format(sarr);
    check_slice_array_empty(res);
}


void check_slice_array_simple(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[size = 2]");
    BOOST_CHECK(res->type() == L"std::slice_array<int>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 3);

    auto sz_child = res->child_at(0);
    BOOST_REQUIRE(sz_child);
    BOOST_CHECK(sz_child->name() == L"[size]");
    BOOST_CHECK(sz_child->val() == L"2");
    BOOST_CHECK(sz_child->type() == L"std::size_t");
    BOOST_CHECK_EQUAL(sz_child->childs_size(), 0);

    auto child1 = res->child_at(1);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[0]");
    BOOST_CHECK(child1->val() == L"10");
    BOOST_CHECK(child1->type() == L"int");
    BOOST_REQUIRE_EQUAL(child1->childs_size(), 0);

    auto child2 = res->child_at(2);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[1]");
    BOOST_CHECK(child2->val() == L"30");
    BOOST_CHECK(child2->type() == L"int");
    BOOST_REQUIRE_EQUAL(child2->childs_size(), 0);
}

/// Tests displaying simple libstdc++ slice_array
BOOST_AUTO_TEST_CASE(stdcpp_slice_array_simple) {
    std::vector<value> vals = {
        make_int(10),
        make_int(20),
        make_int(30),
        make_int(40)
    };
    auto sarr = make_stdcpp_slice_array(cm.bt_int(), vals, 2, 2);
    auto res = ctx.format(sarr);
    check_slice_array_simple(res);
}

/// Tests displaying simple libc++ slice_array
BOOST_AUTO_TEST_CASE(libcxx_slice_array_simple) {
    std::vector<value> vals = {
        make_int(10),
        make_int(20),
        make_int(30),
        make_int(40)
    };
    auto sarr = make_libcxx_slice_array(cm.bt_int(), vals, 2, 2);
    auto res = ctx.format(sarr);
    check_slice_array_simple(res);
}


BOOST_AUTO_TEST_SUITE_END()


}
