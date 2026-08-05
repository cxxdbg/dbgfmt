/// \file template_name_test.cpp
/// Contains unit tests for functions in template_name.hpp

#include "../template_name.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_AUTO_TEST_SUITE(template_name_test)


/// Tests making simple template instantiation name
BOOST_AUTO_TEST_CASE(test_templ_inst_simple) {
    auto res = make_template_inst_name("templ", {"A"});
    BOOST_CHECK(res == "templ<A>");
}


/// Tests making template instantiation name with template arg in single parameter
BOOST_AUTO_TEST_CASE(test_templ_inst_templ_first) {
    auto res = make_template_inst_name("templ", {"A<X>", "B"});
    BOOST_CHECK(res == "templ<A<X>, B>");
}


/// Tests making template instantiation name with template arg in last paramete
BOOST_AUTO_TEST_CASE(test_templ_inst_templ_last) {
    auto res = make_template_inst_name("templ", {"A<X>"});
    BOOST_CHECK(res == "templ<A<X> >");
}


BOOST_AUTO_TEST_SUITE_END()


}
