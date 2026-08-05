
/// \file container_formatter_test.cpp
/// Contains unit tests for the container_formatter class.

#include "error_value.hpp"
#include "fmt_test_fixture.hpp"
#include "../container_formatter.hpp"
#include "dbgfmt/format_error.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


namespace {

/// Container formatter implementation that throws error in element method
class element_throw_container_formatter: public container_formatter {
public:
    bool accepts(const dbgfmt::value & v) const override { return false; }
    std::size_t size(const dbgfmt::value & val) const override { return 1; }

    dbgfmt::value element(const dbgfmt::value & val, std::size_t index) const override {
        assert(index == 0 && "Invalid index");
        throw format_error{"cont_fmt_error"};
    }
};

}


BOOST_FIXTURE_TEST_SUITE(container_formatter_test, fmt_test_fixture)


/// Tests handling exceptions in element method
BOOST_AUTO_TEST_CASE(element_throw) {
    auto fmt = make_formatter<element_throw_container_formatter>();
    fmt.set_fmt_ctx(&ctx);
    auto res = fmt.format(error_value{cm.bt_int()});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 1]");
    BOOST_CHECK(res->type() == L"int");

    BOOST_REQUIRE(res->childs_size() == 2);

    auto child1 = res->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[size]");
    BOOST_CHECK(child1->val() == L"1");
    BOOST_CHECK(child1->type() == L"std::size_t");
    BOOST_CHECK(child1->childs_size() == 0);

    auto child2 = res->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[0]");
    BOOST_CHECK(child2->val() == L"<error: cont_fmt_error>");
    BOOST_CHECK(child2->type() == L"<error>");
    BOOST_CHECK(child2->childs_size() == 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
