
/// \file test2.hpp
/// Contains definitions of value building functions of new test framework.

#pragma once

#include "cm_builder.hpp"
#include "fmt_test_fixture.hpp"
#include "util.hpp"
#include "dbgfmt/value.hpp"
#include <cm/qual_type.hpp>
#include <string_view>


namespace dbgfmt::test {


/// Creates uninitialized value of specified type
value make_val(const cm::qual_type & t);

/// Creates errora value of specified type
value make_err(const cm::qual_type & t);

/// Returns CM type of value
cm::qual_type val_type(const value & v);

/// Returns CM type name of value
std::wstring val_type_name(const value & v);

/// Casts value to another type
value cast_val(const value & v, const cm::qual_type & t);


value make_bool(bool v);

value make_char(char v);
value make_short(short v);
value make_int(int v);
value make_long(long v);
value make_long_long(long long v);

value make_uchar(unsigned char v);
value make_ushort(unsigned short v);
value make_uint(unsigned int v);
value make_ulong(unsigned long v);
value make_ulong_long(unsigned long long v);

value make_float(float v);
value make_double(double v);
value make_long_double(long double v);

value make_wchar(wchar_t v);


/// Makes null pointer value of specified type
value make_null(const cm::qual_type & t);

/// Makes null pointer value to specified type
value make_null_to(const cm::qual_type & t);

/// Makes array value with specified element type and size
value make_arr(const cm::qual_type & elt_type, size_t sz);

/// Makes pointer with specified address value
value make_ptr(const cm::qual_type & type, uint64_t addr);

/// Makes array of characters from string
value make_char_arr(const std::string_view & s);


}
