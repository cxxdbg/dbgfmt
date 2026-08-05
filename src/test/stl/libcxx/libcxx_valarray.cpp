
/// \file stdcpp_valarray.cpp
/// Contains implementations of utility functions for testing libstdc++ valarray
/// and related classes formatters.

#include "libcxx_valarray.hpp"


namespace dbgfmt::test {


/// Creates CM for valarray related structures independent from element type
static auto valarray_common_cm() {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .record("slice", "slice")
                .ivar("__start_", b.bt_unsigned_long())
                .ivar("__size_", b.bt_unsigned_long())
                .ivar("__stride_", b.bt_unsigned_long())
            .end()
        .end().end()
    .build();
}


/// Creates CM for testing valarray of specified type
static auto valarray_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("valarray", "T").record("valarray", elt_type)
                .ivar("__begin_", b.ptype(elt_type))
                .ivar("__end_", b.ptype(elt_type))
            .end().end()

            .templ("slice_array", "T").record("slice_array", elt_type)
                .ivar("__vp_", b.ptype(elt_type))
                .ivar("__size_", b.bt_unsigned_long())
                .ivar("__stride_", b.bt_unsigned_long())
            .end().end()
        .end().end()
    .build();
}


cm::record_type * get_libcxx_valarray_type(const cm::qual_type & elt_type) {
    return valarray_cm(elt_type).rtype("valarray");
}


cm::record_type * get_libcxx_slice_array_type(const cm::qual_type & elt_type) {
    return valarray_cm(elt_type).rtype("slice_array");
}


cm::record_type * get_libcxx_slice_type() {
    return valarray_common_cm().rtype("slice");
}


value make_libcxx_valarray(const cm::qual_type & elt_type, const std::vector<value> & elts) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);
    auto data = make_null(elt_ptr_type);

    // creating array of values
    if (!elts.empty()) {
        auto elt_arr = make_arr(elt_type, elts.size());
        for (size_t i = 0, e = elts.size(); i < e; ++i) {
            elt_arr[i] << elts[i];
        }

        data = elt_arr.ptr().cast(tctx.vctx().get_or_create_type(elt_ptr_type));
    }

    // creating valarray record
    auto varr = make_val(get_libcxx_valarray_type(elt_type));
    varr["__begin_"] << data;
    varr["__end_"] << (data + elts.size());
    return varr;
}


value make_libcxx_slice(size_t offs, size_t size, size_t stride) {
    auto slice = make_val(get_libcxx_slice_type());
    slice["__start_"] << make_ulong(offs);
    slice["__size_"] << make_ulong(size);
    slice["__stride_"] << make_ulong(stride);
    return slice;
}


value make_libcxx_slice_array(const cm::qual_type & elt_type,
                              const std::vector<value> & elts,
                              size_t size,
                              size_t stride) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);
    auto data = make_null(elt_ptr_type);

    if (!elts.empty()) {
        auto elt_arr = make_arr(elt_type.type(), elts.size());
        for (size_t i = 0, e = elts.size(); i < e; ++i) {
            elt_arr[i] << elts[i];
        }

        data = elt_arr.ptr().cast(tctx.vctx().get_or_create_type(elt_ptr_type));
    }

    auto arr = make_val(get_libcxx_slice_array_type(elt_type));
    arr["__size_"] << make_ulong(size);
    arr["__stride_"] << make_ulong(stride);
    arr["__vp_"] << data;
    return arr;
}


}
