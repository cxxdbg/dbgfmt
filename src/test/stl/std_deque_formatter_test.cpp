
/// \file stdcpp_deque_formatter_test.cpp
/// Contains unit tests for deque formatterы.

#include "stdcpp/stdcpp_deque.hpp"
#include "libcxx/libcxx_deque.hpp"
#include "../test2.hpp"
#include "../fmt_test_fixture.hpp"
#include "../util.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(deque_formatter_test, fmt_test_fixture)


/// Checks result of formatting of empty deque
void check_empty_deque(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 0]");
    BOOST_CHECK(res->type() == L"std::deque<int>");

    BOOST_REQUIRE(res->childs_size() == 1);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"0");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(empty_stdcpp_deque) {
    auto deque = make_empty_stdcpp_deque(cm.bt_int(), 10);
    auto res = ctx.format(deque);
    check_empty_deque(res);
}

BOOST_AUTO_TEST_CASE(empty_libcxx_deque) {
    auto deque = make_empty_libcxx_deque(cm.bt_int());
    auto res = ctx.format(deque);
    check_empty_deque(res);
}


/// Checks result of formatting simple deque value
void check_simple_deque(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 1]");
    BOOST_CHECK(res->type() == L"std::deque<int>");

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

BOOST_AUTO_TEST_CASE(simple_stdcpp_deque) {
    std::vector<value> vals;
    vals.push_back(make_int(10));
    auto deque = make_stdcpp_deque(cm.bt_int(), vals, 1);
    auto res = ctx.format(deque);
    check_simple_deque(res);
}

BOOST_AUTO_TEST_CASE(simple_libcxx_deque) {
    std::vector<value> vals;
    vals.push_back(make_int(10));
    auto deque = make_libcxx_deque(cm.bt_int(), vals, 1);
    auto res = ctx.format(deque);
    check_simple_deque(res);
}


/// Checks result of formatting multinode deque
void check_multinode_deque(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 2]");
    BOOST_CHECK(res->type() == L"std::deque<int>");

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

BOOST_AUTO_TEST_CASE(multinode_stdcpp_deque) {
    std::vector<value> vals;
    vals.push_back(make_int(10));
    vals.push_back(make_int(20));
    auto deque = make_stdcpp_deque(cm.bt_int(), vals, 127);
    auto res = ctx.format(deque);
    check_multinode_deque(res);
}

BOOST_AUTO_TEST_CASE(multinode_libcxx_deque) {
    std::vector<value> vals;
    vals.push_back(make_int(10));
    vals.push_back(make_int(20));
    auto deque = make_libcxx_deque(cm.bt_int(), vals, 1025);
    auto res = ctx.format(deque);
    check_multinode_deque(res);
}


/// Checks result of formatting deque with single full node
void check_full_node_deque(const fmt_result_ref & res, size_t sz) {
    std::wostringstream sz_str;
    sz_str << sz;
    auto sz_s = sz_str.str();

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == std::wstring{L"[size = "} + sz_s + L"]");
    BOOST_CHECK(res->type() == L"std::deque<int>");

    BOOST_REQUIRE(res->childs_size() == sz + 1);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == sz_s);
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    for (std::size_t i = 0; i < sz; ++i) {
        auto item = res->child_at(i + 1);

        BOOST_REQUIRE(item);

        std::wostringstream item_name;
        item_name << "[" << i << "]";
        BOOST_CHECK(item->name() == item_name.str());

        std::wostringstream item_val;
        item_val << i;
        BOOST_CHECK(item->val() == item_val.str());

        BOOST_CHECK(item->type() == L"int");
        BOOST_CHECK(item->childs_size() == 0);
    }
}

/// Tests libstdc++ deque with single full node
BOOST_AUTO_TEST_CASE(full_node_stdcpp_deque) {
    std::vector<value> vals;
    for (int i = 0; i < 128; ++i) {
        vals.push_back(make_int(i));
    }

    auto deque = make_stdcpp_deque(cm.bt_int(), vals, 0);
    auto res = ctx.format(deque);
    check_full_node_deque(res, 128);
}

/// Tests libc++ deque with single full node
BOOST_AUTO_TEST_CASE(full_node_libcxx_deque) {
    std::vector<value> vals;
    for (int i = 0; i < 1024; ++i) {
        vals.push_back(make_int(i));
    }

    auto deque = make_libcxx_deque(cm.bt_int(), vals, 0);
    auto res = ctx.format(deque);
    check_full_node_deque(res, 1024);
}


BOOST_AUTO_TEST_CASE(invalid_stdcpp_deque) {
    auto deque_type = get_stdcpp_deque_type(cm.bt_int());
    auto res = ctx.format(make_err(deque_type));
    BOOST_CHECK(is_error_res(res, L"std::deque<int>"));
}

BOOST_AUTO_TEST_CASE(invalid_libcxx_deque) {
    auto deque_type = get_libcxx_deque_type(cm.bt_int());
    auto res = ctx.format(make_err(deque_type));
    BOOST_CHECK(is_error_res(res, L"std::__1::deque<int>"));
}


/// Checks displaying "Raw Data" in invalid deque
void check_invalid_deque_raw_data(const fmt_result_ref & res, const std::wstring & type_name) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(is_error_msg(res->val()));
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE(res->childs_size() == 1);

    auto rd = res->child_at(0);
    BOOST_REQUIRE(rd);
    BOOST_CHECK(rd->name() == L"[Raw Data]");
    BOOST_CHECK(rd->childs_size() > 0);
    BOOST_CHECK(rd->type() == type_name);
}

/// Tests displaying "Raw Data" in invalid libstdc++ deque
BOOST_AUTO_TEST_CASE(invalid_stdcpp_deque_raw_data) {
    auto deque_type = get_stdcpp_deque_type(cm.bt_int());
    ctx.set_show_raw_data(true);
    auto res = ctx.format(make_err(deque_type));
    check_invalid_deque_raw_data(res, L"std::deque<int>");
}

/// Tests displaying "Raw Data" in invalid libc++ deque
BOOST_AUTO_TEST_CASE(invalid_libcxx_deque_raw_data) {
    auto deque_type = get_libcxx_deque_type(cm.bt_int());
    ctx.set_show_raw_data(true);
    auto res = ctx.format(make_err(deque_type));
    check_invalid_deque_raw_data(res, L"std::__1::deque<int>");
}


BOOST_AUTO_TEST_CASE(invalid_max_size_stdcpp_deque) {

    auto elt_type = cm.bt_int();
    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);
    auto elt_ptr_ptr_type = cm.get_or_create_ptr_type(elt_ptr_type);

    auto start = make_stdcpp_deque_iterator(elt_type,
                                            make_null(elt_ptr_type),
                                            make_null(elt_ptr_type),
                                            make_null(elt_ptr_type),
                                            make_null(elt_ptr_ptr_type));

    auto finish = make_stdcpp_deque_iterator(elt_type,
                                             make_null(elt_ptr_type),
                                             make_null(elt_ptr_type),
                                             make_null(elt_ptr_type),
                                             make_null(elt_ptr_ptr_type));

    auto deque = make_stdcpp_deque(elt_type,
                                   make_null(elt_ptr_ptr_type),
                                   make_ulong(5000000),
                                   start,
                                   finish);

    auto res = ctx.format(deque);
    BOOST_CHECK(is_error_res(res, L"std::deque<int>"));
}


/// Checks result of formatting deque iterator
void check_iterator(const fmt_result_ref & res, const std::wstring & type_name) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10");
    BOOST_CHECK(res->type() == type_name);

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ptr = res->child_at(0);

    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_REQUIRE(ptr->childs_size() == 1);

    auto val = ptr->child_at(0);
    BOOST_CHECK(val->name() == L"");
    BOOST_CHECK(val->val() == L"10");
    BOOST_CHECK(val->type() == L"int");
    BOOST_REQUIRE(val->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(stdcpp_deque_iterator) {
    auto ival = make_int(10);
    auto it = make_stdcpp_deque_iterator(cm.bt_int(), ival.address(), 0, 0, 0);
    auto res = ctx.format(it);
    check_iterator(res, L"std::_Deque_iterator<int>");
}

BOOST_AUTO_TEST_CASE(libcxx_deque_iterator) {
    auto ival = make_int(10);
    auto it = make_libcxx_deque_iterator(ival);
    auto res = ctx.format(it);
    check_iterator(res, L"std::__1::__deque_iterator<int>");
}


BOOST_AUTO_TEST_SUITE_END()


}
