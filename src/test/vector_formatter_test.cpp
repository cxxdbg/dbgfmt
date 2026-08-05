
/// \file vector_formatter_test.cpp
/// Contains unit tests for vector types formatter

#include "test2.hpp"
#include "builtin_value.hpp"
#include "fmt_test_fixture.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(vector_formatter_test, fmt_test_fixture)


BOOST_AUTO_TEST_CASE(array_value_test) {
    auto type = cm.get_or_create_vec_type(cm.bt_int(), 4);
    auto val = make_val(type);
    val[0] << make_int(10);
    val[1] << make_int(20);
    val[2] << make_int(30);
    val[3] << make_int(40);

    auto res = ctx.format(val);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[size = 4]");
    BOOST_CHECK(res->type() == L"int __attribute__((vector_size(4)))");
    BOOST_REQUIRE(res->childs_size() == 5);

    auto size_child = res->child_at(0);
    BOOST_REQUIRE(size_child);
    BOOST_CHECK(size_child->name() == L"[size]");
    BOOST_CHECK(size_child->val() == L"4");
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

    auto child3 = res->child_at(4);
    BOOST_REQUIRE(child3);
    BOOST_CHECK(child3->name() == L"[3]");
    BOOST_CHECK(child3->val() == L"40");
    BOOST_CHECK(child3->type() == L"int");
    BOOST_CHECK(child3->childs_size() == 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
