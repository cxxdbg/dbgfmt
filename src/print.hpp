
/// \file print.hpp
/// Contains definition of hex-printing utilities for the fmt library.

#pragma once

#include <format>
#include <ostream>
#include <type_traits>


namespace dbgfmt {


/// Prints integer as hex value with specified size with no prefix
template <typename C, typename T>
void print_hex_no_prefix(std::basic_ostream<C> & str,
                         T x,
                         std::size_t sz,
                         std::size_t min_sz) {

    // converting value to unsigned type
    auto ux = std::make_unsigned_t<T>(x);

    // clearing bits higher than requested size
    decltype(ux) mask = 0xFF;
    if constexpr (sizeof(mask) > 1) {
        for (size_t i = 1; i < sz; ++i) {
            mask <<= 8;
            mask |= static_cast<decltype(ux)>(0xFF);
        }
    }
    ux &= mask;

    std::size_t real_min_sz = min_sz;

    if (min_sz > sz) {
        // use type size as minimum size
        real_min_sz = sz;
    } else {
        // detecting if value fits into minimum size
        decltype(ux) max_val = 255;
        for (auto i = 1; i < min_sz; ++i) {
            max_val *= 255;
        }

        if (x <= max_val) {
            real_min_sz = min_sz;
        } else {
            real_min_sz = sz;
        }
    }

    if constexpr (std::is_same_v<C, char>) {
        str << std::format("{:0{}X}", ux, real_min_sz * 2);
    } else {
        str << std::format(L"{:0{}X}", ux, real_min_sz * 2);
    }
}


/// Prints integer as hex value of specified size with 0x prefix
template <typename C, typename T>
void print_hex(std::basic_ostream<C> & str,
               T x,
               std::size_t sz,
               std::size_t min_sz) {
    str.put(static_cast<C>('0'));
    str.put(static_cast<C>('x'));

    print_hex_no_prefix(str, x, sz, min_sz);
}


}
