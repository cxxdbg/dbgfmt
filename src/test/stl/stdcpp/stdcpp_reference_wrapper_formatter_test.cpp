
/// \file stdcpp_reference_wrapper_formatter_test.cpp
/// Contains unit tests for libstdc++ reference wrapper formatter

#include "../../builtin_value.hpp"
#include "../../cm_builder.hpp"
#include "../../fmt_test_fixture.hpp"
#include "../../pointer_value.hpp"
#include "../../record_value.hpp"
#include "../../util.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(stdcpp_reference_wrapper_formatter_test, fmt_test_fixture)


/// Tests formatting simple reference wrapper
BOOST_AUTO_TEST_CASE(test_simple) {
    cm_builder b;
    auto my_record_type = b
        .record("my_record", "my_record")
            .ivar("v", b.bt_int())
        .end()
    .build().rtype("my_record");

    auto wrapper_type = b
        .ns("std")
            .templ("reference_wrapper", "T").record("wrapper", my_record_type)
                .ivar("_M_data", b.ptype(b.typeref("my_record")))
            .end().end()
        .end()
    .build().rtype("wrapper");

    record_value rec{my_record_type};
    rec["v"] = int_value{10};

    record_value wrapper{wrapper_type};
    wrapper["_M_data"] = rec.ptr();

    auto res = ctx.format(wrapper);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(rec.addr()) + L" { v=10 }");
    BOOST_CHECK(res->type() == L"std::reference_wrapper<my_record>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"v");
    BOOST_CHECK(child->val() == L"10");
    BOOST_CHECK(child->type() == L"int");
    BOOST_CHECK_EQUAL(child->childs_size(), 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
