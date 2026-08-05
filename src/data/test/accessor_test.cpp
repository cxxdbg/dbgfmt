
/// \file accessor_test.cpp
/// Contains unit tests for the accessor class.

#include "dbgfmt/data/accessor.hpp"
#include "dbgfmt/data/vector_storage.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::data::test {


struct accessor_test_fixture {
    vector_storage st;
    accessor acc{st};
};


BOOST_FIXTURE_TEST_SUITE(accessor_test, accessor_test_fixture)


/// Tests reading byte from data
BOOST_AUTO_TEST_CASE(read_unsigned_byte) {
    st.append({0x10});
    auto res = acc.read_unsigned(1);
    BOOST_CHECK_EQUAL(res, 16);
}


/// Tests reading 2 bytes from data
BOOST_AUTO_TEST_CASE(read_unsigned_2_bytes) {
    st.append({0x10, 0x20});
    auto res = acc.read_unsigned(2);
    BOOST_CHECK_EQUAL(res, 0x2010);
}


/// Tests reading signed byte
BOOST_AUTO_TEST_CASE(read_signed_byte) {
    st.append({0xE1});
    BOOST_CHECK_EQUAL(acc.read_signed(1), -31);
}


/// Tests reading signed 2 bytes
BOOST_AUTO_TEST_CASE(read_signed_2_bytes) {
    st.append({0x20, 0xE0});
    auto res = acc.read_signed(2);
    BOOST_CHECK_EQUAL(res, -8160);
}


/// Tests reading float value
BOOST_AUTO_TEST_CASE(read_float) {
    st.append({0x64, 0x76, 0xf6, 0x42});
    BOOST_CHECK_EQUAL(acc.read_float(4), 123.23123f);
}


/// Tests reading 2 byte value in big endian
BOOST_AUTO_TEST_CASE(read_unsigned_2_bytes_big) {
    st.append({0x20, 0x10});
    acc.set_endian(endianness::big);
    auto res = acc.read_unsigned(2);
    BOOST_CHECK_EQUAL(res, 0x2010);
}


/// Tests writing byte
BOOST_AUTO_TEST_CASE(write_unsigned_byte) {
    st.append({0, 0});
    acc.write_unsigned(16, 1);
    BOOST_CHECK((st.data() == std::vector<unsigned char>{0x10, 0}));
}


/// Tests writing 2 bytes
BOOST_AUTO_TEST_CASE(write_unsigned_2_bytes) {
    st.append({0, 0, 0});
    acc.write_unsigned(0x2010, 2);
    BOOST_CHECK((st.data() == std::vector<unsigned char>{0x10, 0x20, 0}));
}


/// Tests writing signed byte
BOOST_AUTO_TEST_CASE(write_signed_byte) {
    st.append({0, 0});
    acc.write_signed(-31, 1);
    BOOST_CHECK((st.data() == std::vector<unsigned char>{0xE1, 0}));
}


/// Tests writing signed 2 bytes
BOOST_AUTO_TEST_CASE(write_signed_2_bytes) {
    st.append({0, 0, 0});
    acc.write_signed(-8160, 2);
    BOOST_CHECK((st.data() == std::vector<unsigned char>{0x20, 0xE0, 0}));
}


/// Tests writing float value
BOOST_AUTO_TEST_CASE(write_float) {
    st.append({0, 0, 0, 0, 0});
    acc.write_float(123.23123f, 4);
    BOOST_CHECK((st.data() == std::vector<unsigned char>{0x64, 0x76, 0xf6, 0x42, 0}));
}


/// Tests writing 2 byte value in big endian
BOOST_AUTO_TEST_CASE(write_unsigned_2_bytes_big) {
    st.append({0, 0, 0});
    acc.set_endian(endianness::big);
    acc.write_unsigned(0x2010, 2);
    BOOST_CHECK((st.data() == std::vector<unsigned char>{0x20, 0x10, 0}));
}


/// Tests reading unsigned bits
BOOST_AUTO_TEST_CASE(read_unsigned_bits) {
    st.append({0U, 0b11101010U, 0b00101100, 0b00100010});
    auto res = acc.read_unsigned_bits(12, 1, 6);
    BOOST_CHECK_EQUAL(res, 0x8B3);
}


/// Tests reading unsigned bits from beginnig
BOOST_AUTO_TEST_CASE(read_unsigned_bits_beginning) {
    st.append({0b11101010U, 0b00101100, 0b00100010});
    auto res = acc.read_unsigned_bits(6, 0, 0);
    BOOST_CHECK_EQUAL(res, 0x2A);
}


/// Tests reading signed bits
BOOST_AUTO_TEST_CASE(read_signed_bits) {
    st.append({0U, 0b11101010U, 0b00101100, 0b00100010});
    // 1000 1011 0011 (-1869)
    auto res = acc.read_signed_bits(12, 1, 6);
    BOOST_CHECK_EQUAL(res, -1869);
}


BOOST_AUTO_TEST_SUITE_END()


}
