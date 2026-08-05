
/// \file test2.cpp
/// Contains implementation of value building functions of new test framework.

#include "test2.hpp"
#include "test_context.hpp"
#include "dbgfmt/simple_value_impl.hpp"
#include "../value_type_impl_accessor.hpp"


namespace dbgfmt::test {


value make_val(const cm::qual_type & t) {
    auto & tctx = test::test_context::current_context();
    auto data = tctx.alloc_data(t);
    auto type_impl = tctx.vctx().get_or_create_type(t);
    auto val_impl = std::make_shared<simple_value_impl>(tctx.vctx(),
                                                        type_impl,
                                                        data.stor(),
                                                        data.endian(),
                                                        data.offset());
    return value{val_impl};
}


value make_err(const cm::qual_type & t) {
    auto & tctx = test::test_context::current_context();
    auto data = tctx.alloc_error_data(t);
    auto type_impl = tctx.vctx().get_or_create_type(t);
    auto val_impl = std::make_shared<simple_value_impl>(tctx.vctx(),
                                                        type_impl,
                                                        data.stor(),
                                                        data.endian(),
                                                        data.offset());
    return value{val_impl};
}


cm::qual_type val_type(const value & v) {
    auto type_impl = value_type_impl_accessor(v.type()).get();
    auto fmtcm_type_impl = dynamic_cast<const dbgfmt::backend::cm::type_impl*>(type_impl);
    assert(fmtcm_type_impl && "value type implementation is not a fmtcm type");
    return fmtcm_type_impl->get_cm_type();
}


std::wstring val_type_name(const value & v) {
    std::ostringstream str;
    cm::cxx::print_type(str, val_type(v));
    auto s = str.str();
    return std::wstring{s.begin(), s.end()};
}


value cast_val(const value & v, const cm::qual_type & t) {
    auto & tctx = test::test_context::current_context();
    auto & vctx = tctx.vctx();
    auto impl = vctx.get_or_create_type(t);
    return v.cast(value_type{impl});
}


template <typename Type>
value make_val(const cm::qual_type & t, const Type & v) {
    auto val = make_val(t);
    val << v;
    return val;
}


#define DEFINE_MAKE_FUNCTION(name, type, cm_type) \
value make_##name(type v) { \
    return make_val(test::test_context::current_context().cm().cm_type(), v); \
}


DEFINE_MAKE_FUNCTION(bool, bool, bt_bool)
DEFINE_MAKE_FUNCTION(char, char, bt_char)
DEFINE_MAKE_FUNCTION(short, short, bt_short)
DEFINE_MAKE_FUNCTION(int, int, bt_int)
DEFINE_MAKE_FUNCTION(long, long, bt_long)
DEFINE_MAKE_FUNCTION(long_long, long long, bt_long)

DEFINE_MAKE_FUNCTION(uchar, unsigned char, bt_unsigned_char)
DEFINE_MAKE_FUNCTION(ushort, unsigned short, bt_unsigned_short)
DEFINE_MAKE_FUNCTION(uint, unsigned int, bt_unsigned_int)
DEFINE_MAKE_FUNCTION(ulong, unsigned long, bt_unsigned_long)
DEFINE_MAKE_FUNCTION(ulong_long, unsigned long long, bt_unsigned_long_long)

DEFINE_MAKE_FUNCTION(float, float, bt_float)
DEFINE_MAKE_FUNCTION(double, double, bt_double)
DEFINE_MAKE_FUNCTION(long_double, long double, bt_long_double)

DEFINE_MAKE_FUNCTION(wchar, wchar_t, bt_wchar_t)


value make_null(const cm::qual_type & t) {
    auto res = make_val(t);
    res << 0;
    return res;
}


value make_null_to(const cm::qual_type & t) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto ptr_type = cm.get_or_create_ptr_type(t);
    return make_null(ptr_type);
}


value make_arr(const cm::qual_type & elt_type, size_t sz) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto arr_t = cm.get_or_create_arr_type(elt_type.type(), sz);
    return make_val(arr_t);
}


value make_ptr(const cm::qual_type & type, uint64_t addr) {
    auto val = make_val(type);
    val << addr;
    return val;
}


value make_char_arr(const std::string_view & s) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto val = make_arr(cm.bt_char(), s.size());
    for (size_t i = 0, sz = s.size(); i < sz; ++i) {
        val[i] << make_char(s[i]);
    }
    return val;
}


}
