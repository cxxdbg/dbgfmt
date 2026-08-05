
/// \file memory_test.cpp
/// Contains unit tests for the memory class.

#include "memory.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


struct memory_test_fixture {
    memory mem;
};


BOOST_FIXTURE_TEST_SUITE(memory_test, memory_test_fixture)


/// Tests reading memory from single block
BOOST_AUTO_TEST_CASE(read_simple) {
    auto block = mem.alloc_block(100, 10);
    block->write(0, {10, 20, 30, 40, 50, 60, 70, 80});
    BOOST_CHECK((mem.read_vector(102, 4) == std::vector<uint8_t>{30, 40, 50, 60}));
}


/// Tests reading memory from multiple blocks
BOOST_AUTO_TEST_CASE(read_multiple_blocks) {
    auto block1 = mem.alloc_block(100, 2);
    block1->write(0, {10, 20});
    auto block2 = mem.alloc_block(102, 3);
    block2->write(0, {30, 40, 50});
    auto block3 = mem.alloc_block(105, 4);
    block3->write(0, {60, 70, 80, 90});


    BOOST_CHECK((mem.read_vector(101, 5) == std::vector<uint8_t>{20, 30, 40, 50, 60}));
}


/// Tests writing memory to single block
BOOST_AUTO_TEST_CASE(write_simple) {
    auto block = mem.alloc_block(100, 10);
    block->write(0, {10, 20, 30, 40, 50, 60, 70, 80});
    mem.write_vector(102, {3, 4, 5});

    BOOST_CHECK((block->read_vector(0, 8) ==
                 std::vector<uint8_t>{10, 20, 3, 4, 5, 60, 70, 80}));
}


/// Tests writing memory to multiple blocks
BOOST_AUTO_TEST_CASE(write_multiple_blocks) {
    auto block1 = mem.alloc_block(100, 2);
    block1->write(0, {10, 20});
    auto block2 = mem.alloc_block(102, 3);
    block2->write(0, {30, 40, 50});
    auto block3 = mem.alloc_block(105, 4);
    block3->write(0, {60, 70, 80, 90});


    mem.write_vector(101, {2, 3, 4, 5, 6});

    BOOST_CHECK((block1->read_vector(0, 2) == std::vector<uint8_t>{10, 2}));
    BOOST_CHECK((block2->read_vector(0, 3) == std::vector<uint8_t>{3, 4, 5}));
    BOOST_CHECK((block3->read_vector(0, 4) == std::vector<uint8_t>{6, 70, 80, 90}));
}


BOOST_AUTO_TEST_SUITE_END()


}
