
/// \file record_value_test.cpp
/// Contains unit tests for libstd++ vector formatter

#include "../test2.hpp"
#include "std_vector_utils.hpp"
#include "stdcpp/stdcpp_normal_iterator.hpp"
#include "stdcpp/stdcpp_vector.hpp"
#include "libcxx/libcxx_vector.hpp"
#include "libcxx/libcxx_wrap_iter.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(stdcpp_vector_formatter, fmt_test_fixture)


/// Checks formatting result for empty vector
static void check_empty_vector(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 0, capacity = 0]");
    BOOST_CHECK(res->type() == L"std::vector<int>");

    BOOST_REQUIRE(res->childs_size() == 2);

    auto child1 = res->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[size]");
    BOOST_CHECK(child1->val() == L"0");
    BOOST_CHECK(child1->type() == L"std::size_t");

    auto child2 = res->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[capacity]");
    BOOST_CHECK(child2->val() == L"0");
    BOOST_CHECK(child2->type() == L"std::size_t");
}


/// Tests formatting empty libstdc++ vector
BOOST_AUTO_TEST_CASE(int_stdcpp_vector_empty_test) {
    auto v = make_stdcpp_vector(cm.bt_int(), {});
    auto res = ctx.format(v);
    check_empty_vector(res);
}


/// Tests formatting empty libc++ vector
BOOST_AUTO_TEST_CASE(int_libcxx_vector_empty_test) {
    auto v = tcm::libcxx::vector::make(cm.bt_int(), {});
    auto res = ctx.format(v);
    check_empty_vector(res);
}


/// Checks formatting result for 2 int vector
static void check_int_vector(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 2, capacity = 2]");
    BOOST_CHECK(res->type() == L"std::vector<int>");

    BOOST_REQUIRE(res->childs_size() == 4);

    auto child1 = res->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[size]");
    BOOST_CHECK(child1->val() == L"2");
    BOOST_CHECK(child1->type() == L"std::size_t");
    BOOST_CHECK(child1->childs_size() == 0);

    auto child2 = res->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[capacity]");
    BOOST_CHECK(child2->val() == L"2");
    BOOST_CHECK(child2->type() == L"std::size_t");
    BOOST_CHECK(child2->childs_size() == 0);

    auto child3 = res->child_at(2);
    BOOST_REQUIRE(child3);
    BOOST_CHECK(child3->name() == L"[0]");
    BOOST_CHECK(child3->val() == L"10");
    BOOST_CHECK(child3->type() == L"int");

    auto child4 = res->child_at(3);
    BOOST_REQUIRE(child4);
    BOOST_CHECK(child4->name() == L"[1]");
    BOOST_CHECK(child4->val() == L"20");
    BOOST_CHECK(child4->type() == L"int");
}


/// Tests formatting libstdc++ vector with 2 elements
BOOST_AUTO_TEST_CASE(int_stdcpp_vector_test) {
    auto v = make_stdcpp_vector(cm.bt_int(), {make_int(10), make_int(20)});
    auto res = ctx.format(v);
    check_int_vector(res);
}


/// Tests formatting libc++ vector with 2 elements
BOOST_AUTO_TEST_CASE(int_libcxx_vector_test) {
    auto v = tcm::libcxx::vector::make(cm.bt_int(), {make_int(10), make_int(20)});
    auto res = ctx.format(v);
    check_int_vector(res);
}


/// Tests displaying very large vector
BOOST_AUTO_TEST_CASE(large_int_vector) {
    std::vector<value> vals;
    for (int i = 0; i < 11000; ++i) {
        vals.push_back(make_int(10));
    }

    auto v = make_stdcpp_vector(cm.bt_int(), vals);
    auto res = ctx.format(v);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"<error: container is very large (size = 11000)>");
    BOOST_CHECK(res->type() == L"std::vector<int>");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Checks result of formatting vector iterator
static void check_vector_iterator(const fmt_result_ref & res, uint64_t addr) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ptr = res->child_at(0);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(addr) + L" 10");

    BOOST_REQUIRE(ptr->childs_size() == 1);
    auto val = ptr->child_at(0);
    BOOST_CHECK(val->name() == L"");
    BOOST_CHECK(val->val() == L"10");
    BOOST_CHECK(val->childs_size() == 0);
}


/// Tests libstdc++ vector iterator
BOOST_AUTO_TEST_CASE(int_stdcpp_vector_iterator) {
    auto vcm = stdcpp_vector_cm(cm.bt_int());
    auto ival = make_int(10);
    auto it = create_stdcpp_normal_iterator(cm.bt_int(), ival.ptr());
    auto res = ctx.format(it);
    check_vector_iterator(res, ival.address());
}


/// Tests libc++ vector iterator
BOOST_AUTO_TEST_CASE(int_libcxx_vector_iterator) {
    auto ival = make_int(10);
    auto it = tcm::libcxx::wrap_iter::make(cm.bt_int(), ival.ptr());
    auto res = ctx.format(it);
    check_vector_iterator(res, ival.address());
}


/// Checks formatting result of vector pointing to invalid memory
static void check_error_vector(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 2, capacity = 2]");
    BOOST_CHECK(res->type() == L"std::vector<int>");

    BOOST_REQUIRE(res->childs_size() == 4);

    auto child1 = res->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[size]");
    BOOST_CHECK(child1->val() == L"2");
    BOOST_CHECK(child1->type() == L"std::size_t");
    BOOST_CHECK(child1->childs_size() == 0);

    auto child2 = res->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[capacity]");
    BOOST_CHECK(child2->val() == L"2");
    BOOST_CHECK(child2->type() == L"std::size_t");
    BOOST_CHECK(child2->childs_size() == 0);

    auto child3 = res->child_at(2);
    BOOST_CHECK(is_error_res(child3, L"int", L"[0]"));

    auto child4 = res->child_at(3);
    BOOST_CHECK(is_error_res(child4, L"int", L"[1]"));
}


/// Tests formatting libstdc++ vector containing error value
BOOST_AUTO_TEST_CASE(error_stdcpp_vector_test) {
    auto elt_ptr_type = cm.get_or_create_ptr_type(cm.bt_int());
    auto arr_type = cm.get_or_create_arr_type(cm.bt_int(), 2);
    auto eval = make_err(arr_type);

    auto begin = eval.ptr().cast(vctx.get_or_create_type(elt_ptr_type));
    auto end = (eval.ptr() + 1).cast(vctx.get_or_create_type(elt_ptr_type));

    auto v = make_stdcpp_vector_begin_end(cm.bt_int(), begin, end);
    auto res = ctx.format(v);

    check_error_vector(res);
}


/// Tests formatting libcxx vector containing error value
BOOST_AUTO_TEST_CASE(error_libcxx_vector_test) {
    auto elt_ptr_type = cm.get_or_create_ptr_type(cm.bt_int());
    auto arr_type = cm.get_or_create_arr_type(cm.bt_int(), 2);
    auto eval = make_err(arr_type);

    auto begin = eval.ptr().cast(vctx.get_or_create_type(elt_ptr_type));
    auto end = (eval.ptr() + 1).cast(vctx.get_or_create_type(elt_ptr_type));

    auto v = tcm::libcxx::vector::make(cm.bt_int(), begin, end);
    auto res = ctx.format(v);

    check_error_vector(res);
}


BOOST_AUTO_TEST_SUITE_END()

}
