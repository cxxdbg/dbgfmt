
/// \file array_formatter_test.cpp
/// Contains unit tests for plain array formatter

#include "array_value.hpp"
#include "builtin_value.hpp"
#include "fmt_test_fixture.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(array_formatter_test, fmt_test_fixture)


BOOST_AUTO_TEST_CASE(array_value_test) {
    array_value arr{cm.bt_int(), 3};
    arr[0] = int_value{10};
    arr[1] = int_value{20};
    arr[2] = int_value{30};

    auto res = ctx.format(arr);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[size = 3]");
    BOOST_CHECK(res->type() == L"int [3]");
    BOOST_REQUIRE(res->childs_size() == 4);

    auto size_child = res->child_at(0);
    BOOST_REQUIRE(size_child);
    BOOST_CHECK(size_child->name() == L"[size]");
    BOOST_CHECK(size_child->val() == L"3");
    BOOST_CHECK(size_child->type() == L"std::size_t");
    BOOST_CHECK(size_child->childs_size() == 0);

    auto child0 = res->child_at(1);
    BOOST_REQUIRE(child0);
    BOOST_CHECK(child0->name() == L"[0]");
    BOOST_CHECK(child0->val() == L"10");
    BOOST_CHECK(child0->type() == L"int");
    BOOST_CHECK(child0->childs_size() == 0);

    auto child1 = res->child_at(2);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[1]");
    BOOST_CHECK(child1->val() == L"20");
    BOOST_CHECK(child1->type() == L"int");
    BOOST_CHECK(child1->childs_size() == 0);

    auto child2 = res->child_at(3);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[2]");
    BOOST_CHECK(child2->val() == L"30");
    BOOST_CHECK(child2->type() == L"int");
    BOOST_CHECK(child2->childs_size() == 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
