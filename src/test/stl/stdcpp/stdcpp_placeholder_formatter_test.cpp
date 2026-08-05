
/// \file stdcpp_placeholder_formatter_test.cpp
/// Contains unit tests for formatting libstdc++ placeholders

#include "stdcpp_bind.hpp"
#include "../../fmt_test_fixture.hpp"
#include "../../record_value.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(stdcpp_placeholder_formatter_test, fmt_test_fixture)


/// Tests formatting placeholder
BOOST_AUTO_TEST_CASE(placeholder) {
    auto type = get_stdcpp_placeholder_type(cm::value{"2"});
    record_value rec{type};
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"_2");
    BOOST_CHECK(res->type() == L"std::_Placeholder<2>");
    BOOST_CHECK_EQUAL(res->childs_size(), 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
