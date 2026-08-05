
/// \file builtin_value.hpp
/// Contains definition of builtin_value class.

#pragma once

#include "value.hpp"


namespace dbgfmt::test {


/// Builtin value builder
template <cm::builtin_type::kind_t Kind, typename Type>
class builtin_value: public value<cm::builtin_type> {
public:
    /// Constructs builder of value
    builtin_value(test_context & ctx, Type val):
    value(ctx, ctx.cm().bt_type(Kind)) {
        data().write(val, size());
    }

    /// Constructs builder of value
    builtin_value(Type val):
        builtin_value(test_context::current_context(), val) {}

    /// Constructs builder for builtin value with specified data
    builtin_value(test_context & ctx, const qual_type & type, const data::accessor & d):
        value<cm::builtin_type>(ctx, type, d) {}

    /// Reads value as unsigned int64
    uint64_t as_uint64() {
        return data().read_unsigned(size());
    }

    /// Writes unsigned int64 to value data cuttin it by value size
    void set_uint64(uint64_t val) {
        data().write(val, size());
    }
};


/// Complex value builder
template <cm::builtin_type::kind_t Kind, typename Type>
class complex_value: public value<cm::builtin_type> {
public:
    /// Constructs builder of value
    complex_value(test_context & ctx, Type real, Type img):
    value(ctx, ctx.cm().bt_type(Kind)) {
        auto sz = size() / 2;
        data().write(real, sz);
        data().write(img, sz, sz);
    }

    /// Constructs builder of complex value
    complex_value(Type real, Type img):
        complex_value(test_context::current_context(), real, img) {}
};


template <cm::builtin_type::kind_t Kind>
using signed_builtin_value = builtin_value<Kind, int64_t>;

template <cm::builtin_type::kind_t Kind>
using unsigned_builtin_value = builtin_value<Kind, uint64_t>;

template <cm::builtin_type::kind_t Kind>
using float_builtin_value = builtin_value<Kind, long double>;

template <cm::builtin_type::kind_t Kind>
using signed_complex_value = complex_value<Kind, int64_t>;

template <cm::builtin_type::kind_t Kind>
using unsigned_complex_value = complex_value<Kind, uint64_t>;

template <cm::builtin_type::kind_t Kind>
using float_complex_value = complex_value<Kind, long double>;


using bool_value = unsigned_builtin_value<cm::builtin_type::kind_t::bool_>;

using char_value = signed_builtin_value<cm::builtin_type::kind_t::char_>;
using short_value = signed_builtin_value<cm::builtin_type::kind_t::short_>;
using int_value = signed_builtin_value<cm::builtin_type::kind_t::int_>;
using long_value = signed_builtin_value<cm::builtin_type::kind_t::long_>;
using long_long_value = signed_builtin_value<cm::builtin_type::kind_t::long_long_>;

using unsigned_char_value = unsigned_builtin_value<cm::builtin_type::kind_t::unsigned_char_>;
using unsigned_short_value = unsigned_builtin_value<cm::builtin_type::kind_t::unsigned_short_>;
using unsigned_int_value = unsigned_builtin_value<cm::builtin_type::kind_t::unsigned_int_>;
using unsigned_long_value = unsigned_builtin_value<cm::builtin_type::kind_t::unsigned_long_>;
using unsigned_long_long_value = unsigned_builtin_value<cm::builtin_type::kind_t::unsigned_long_long_>;

using float_value = float_builtin_value<cm::builtin_type::kind_t::float_>;
using double_value = float_builtin_value<cm::builtin_type::kind_t::float_>;
using long_double_value = float_builtin_value<cm::builtin_type::kind_t::float_>;


using wchar_value = signed_builtin_value<cm::builtin_type::kind_t::wchar_t_>;


using complex_char_value = signed_complex_value<cm::builtin_type::kind_t::complex_char_>;
using complex_short_value = signed_complex_value<cm::builtin_type::kind_t::complex_short_>;
using complex_int_value = signed_complex_value<cm::builtin_type::kind_t::complex_int_>;
using complex_long_value = signed_complex_value<cm::builtin_type::kind_t::complex_long_>;
using complex_long_long_value = signed_complex_value<cm::builtin_type::kind_t::complex_long_long_>;

using complex_unsigned_char_value = unsigned_complex_value<cm::builtin_type::kind_t::complex_unsigned_char_>;
using complex_unsigned_short_value = unsigned_complex_value<cm::builtin_type::kind_t::complex_unsigned_short_>;
using complex_unsigned_int_value = unsigned_complex_value<cm::builtin_type::kind_t::complex_unsigned_int_>;
using complex_unsigned_long_value = unsigned_complex_value<cm::builtin_type::kind_t::complex_unsigned_long_>;
using complex_unsigned_long_long_value = unsigned_complex_value<cm::builtin_type::kind_t::complex_unsigned_long_long_>;

using complex_float_value = float_complex_value<cm::builtin_type::kind_t::complex_float_>;
using complex_double_value = float_complex_value<cm::builtin_type::kind_t::complex_double_>;
using complex_long_double_value = float_complex_value<cm::builtin_type::kind_t::complex_long_double_>;



#define CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(value_type, builtin_type) \
    template <typename Type> \
    value<Type> & value<Type>::operator=(builtin_type v) { \
        return *this = value_type{v}; \
    }


CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(char_value, char)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(short_value, short)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(int_value, int)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(long_value, long)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(long_long_value, long long)

CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(unsigned_char_value, unsigned char)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(unsigned_short_value, unsigned short)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(unsigned_int_value, unsigned int)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(unsigned_long_value, unsigned long)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(unsigned_long_long_value, unsigned long long)

CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(float_value, float)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(double_value, double)
CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(long_double_value, long double)

CXXDBG_FMT_TEST_DEFINE_VALUE_ASSIGN_OP(wchar_value, wchar_t)


}


