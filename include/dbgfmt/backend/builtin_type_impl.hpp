
/// \file builtin_type_impl.hpp
/// Contains definition of the builtin_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


/// Represents abstract builtin type implementation
class builtin_type_impl: virtual public type_impl {
public:
    /// Kind of builtin type
    enum class kind_t {
        void_,
        bool_,
        char_,
        short_,
        int_,
        long_,
        long_long_,
        unsigned_char_,
        unsigned_short_,
        unsigned_int_,
        unsigned_long_,
        unsigned_long_long_,
        float_,
        double_,
        long_double_,

        wchar_t_,
        char16_t_,
        char32_t_,

        nullptr_t_,

        complex_char_,
        complex_short_,
        complex_int_,
        complex_long_,
        complex_long_long_,
        complex_unsigned_char_,
        complex_unsigned_short_,
        complex_unsigned_int_,
        complex_unsigned_long_,
        complex_unsigned_long_long_,
        complex_float_,
        complex_double_,
        complex_long_double_,

        num_types_
    };

    /// Returns kind of builtin type
    virtual kind_t kind() const = 0;
};


}


