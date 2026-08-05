
/// \file auto_memory_test.cpp
/// Contains unit tests fot the auto_memory class.

#include "auto_memory.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


struct auto_memory_test_fixture {
    auto_memory mem;
};


BOOST_FIXTURE_TEST_SUITE(auto_memory_test, auto_memory_test_fixture)


/// Tests alloc
BOOST_AUTO_TEST_CASE(test_alloc) {
    auto offs1 = mem.alloc(2);
    auto offs2 = mem.alloc(2);
    BOOST_CHECK(offs2 == offs1 + 2 + 100);
}


BOOST_AUTO_TEST_SUITE_END()


}
