
/// \file endianness.hpp
/// Contains endianness constants definitions.

#pragma once

#include <boost/predef/other/endian.h>


namespace dbgfmt::data {


/// Endianness used by data readers and writers
enum class endianness {
    little,
    big
};


/// Returns host endianness
constexpr endianness host_endianness() noexcept {
#if BOOST_ENDIAN_BIG_BYTE
    return endianness::big;
#elif BOOST_ENDIAN_LITTLE_BYTE
    return endianness::little;
#else
#error "Unknown host endianness"
#endif
}


}
