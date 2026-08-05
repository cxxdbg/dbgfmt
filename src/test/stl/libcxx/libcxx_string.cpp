
/// \file libcxx_string.cpp
/// Contains implementation of functions for creating libc++ string types and values.

#include "libcxx_string.hpp"


namespace dbgfmt::test {


/// Returns count of characters in inline libc++ string representation for
/// specified character type
static size_t libcxx_short_rep_count(const cm::qual_type & elt_type) {
    auto & tctx = test_context::current_context();
    auto & dbg = tctx.dbg();
    auto & cm = tctx.cm();

    size_t long_rep_size = dbg.type_size(cm.bt_unsigned_long()) * 2 + dbg.ptr_size();
    size_t num_chars = (long_rep_size - 1) / dbg.type_size(elt_type);
    if (num_chars > 2) {
        return num_chars;
    } else {
        return 2;
    }
}


/// Returns true if libc++ string should store data inline
static bool is_libcxx_string_inline(const cm::qual_type & elt_type, size_t sz) {
    return sz < libcxx_short_rep_count(elt_type);
}


cm::builder_result libcxx_string_cm(const cm::qual_type & elt_type, bool alternate_layout) {

    auto & ctx = test_context::current_context();
    auto & dbg = ctx.dbg();

    const char * long_field_1_name = nullptr;
    const char * long_field_2_name = nullptr;
    const char * long_field_3_name = nullptr;

    cm::type_t * long_field_1_type = nullptr;
    cm::type_t * long_field_2_type = nullptr;
    cm::type_t * long_field_3_type = nullptr;

    const char * short_field_1_name = nullptr;
    const char * short_field_2_name = nullptr;

    cm::type_t * short_field_1_type = nullptr;
    cm::type_t * short_field_2_type = nullptr;

    auto short_array_type = ctx.cm().get_or_create_arr_type(elt_type.type(), libcxx_short_rep_count(elt_type));

    if (alternate_layout) {
        // this layout is used on macos
        long_field_1_name = "__data_";
        long_field_3_name = "__cap_";

        long_field_1_type = ctx.cm().get_or_create_ptr_type(elt_type);
        long_field_3_type = ctx.cm().bt_unsigned_long();

        short_field_1_name = "__data_";
        short_field_2_name = "__size_";

        short_field_1_type = short_array_type;
        short_field_2_type = ctx.cm().bt_unsigned_char();
    } else {
        long_field_1_name = "__cap_";
        long_field_3_name = "__data_";

        long_field_1_type = ctx.cm().bt_unsigned_long();
        long_field_3_type = ctx.cm().get_or_create_ptr_type(elt_type);

        short_field_1_name = "__size_";
        short_field_2_name = "__data_";

        short_field_1_type = ctx.cm().bt_unsigned_char();
        short_field_2_type = short_array_type;
    }

    size_t padding_size = dbg.type_size(elt_type) - 1;

    if (padding_size == 0) {
        cm_builder b;
        return b
            .ns("std").ns("__1")
                .templ("basic_string", "Char").record("string", elt_type)
                    .record("__long", "long_value")
                        .ivar(long_field_1_name, long_field_1_type)
                        .ivar("__size_", b.bt_unsigned_long())
                        .ivar(long_field_3_name, long_field_3_type)
                    .end()

                    .record("__short", "short_value")
                        .ivar(short_field_1_name, short_field_1_type)
                        .ivar(short_field_2_name, short_field_2_type)
                    .end()

                    .record("__rep", "rep")
                        .record(cm::record_kind::union_, "value_union")
                            .ivar("__l", b.typeref("long_value"))
                            .ivar("__s", b.typeref("short_value"))
                        .end()

                        .ivar("__value_", b.typeref("value_union"))
                    .end()

                    .ivar("__r_", b.typeref("rep"))
                .end().end()
            .end().end()
        .build();
    } else {
        cm_builder b;
        return b
            .ns("std").ns("__1")
                .templ("basic_string", "Char").record("string", elt_type)
                    .record("__long", "long_value")
                        .ivar(long_field_1_name, long_field_1_type)
                        .ivar("__size_", b.bt_unsigned_long())
                        .ivar(long_field_3_name, long_field_3_type)
                    .end()

                    .record("__short", "short_value")
                        .ivar(short_field_1_name, short_field_1_type)
                        .ivar("__padding_", b.atype(b.bt_unsigned_char(), padding_size))
                        .ivar(short_field_2_name, short_field_2_type)
                    .end()

                    .record("__rep", "rep")
                        .record(cm::record_kind::union_, "value_union")
                            .ivar("__l", b.typeref("long_value"))
                            .ivar("__s", b.typeref("short_value"))
                        .end()

                        .ivar("__value_", b.typeref("value_union"))
                    .end()

                    .ivar("__r_", b.typeref("rep"))
                .end().end()
            .end().end()
        .build();
    }
}


/// Creates libc++ string
value create_libcxx_string(const cm::qual_type & char_type,
                           const std::vector<value> & chars,
                           bool alternate_layout) {

    // TODO: implement correct formatting and testing for libc++ string for big endian (CXXDBG-757)

    unsigned long long_mask = 0;

    if (alternate_layout) {
        // TODO: how to handle different size of long type in test CM? (32bit)
        long_mask = ~(((unsigned long)(~0)) >> 1);
    } else {
        long_mask = 0x01;
    }


    auto cmres = libcxx_string_cm(char_type, alternate_layout);
    auto res = make_val(cmres.rtype("string"));

    if (is_libcxx_string_inline(char_type, chars.size())) {
        auto c_size = static_cast<unsigned char>(chars.size());

        if (!alternate_layout) {
            c_size <<= 1;
        }

        res["__r_"]["__value_"]["__s"]["__size_"] << make_uchar(c_size);

        for (size_t i = 0, e = chars.size(); i < e; ++i) {
            res["__r_"]["__value_"]["__s"]["__data_"][i] << chars[i];
        }

        res["__r_"]["__value_"]["__s"]["__data_"][chars.size()] << '\0';
    } else {
        // capacity can't be odd because it indicates short inline string
        auto cap = chars.size() + 100;
        if (cap % 2 != 0) {
            ++cap;
        }

        auto data = make_arr(char_type, cap);
        for (auto i = 0; i < chars.size(); ++i) {
            data[i] << chars[i];
        }

        data[chars.size()] << '\0';

        cap |= long_mask;

        res["__r_"]["__value_"]["__l"]["__cap_"] << make_ulong(cap);
        res["__r_"]["__value_"]["__l"]["__size_"] << make_ulong(chars.size());
        res["__r_"]["__value_"]["__l"]["__data_"] << data.ptr();
    }

    return res;
}


}
