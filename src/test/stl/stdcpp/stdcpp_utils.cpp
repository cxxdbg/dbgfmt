
/// \file stdcpp_utils.cpp
/// Contains implementation of utility functions for libstdc++
/// formatters tests.

#include "stdcpp_utils.hpp"
#include "../../cm_builder.hpp"
#include <cm/builder.hpp>
#include <sstream>


namespace dbgfmt::test {


cm::record_type * get_stdcpp_aligned_membuf_type(const cm::qual_type & type) {
    cm_builder b;
    auto res = b
        .ns("__gnu_cxx")
            .templ("__aligned_membuf", "T").record("membuf", type)
                .ivar("_M_storage", b.atype(b.bt_char(), b.dbg().type_size(type)))
            .end().end()
        .end()
    .build();

    return res.type("membuf")->cast<cm::record_type>();
}


cm::record_type * get_stdcpp_aligned_storage_type(size_t sz) {
    cm_builder b;

    std::ostringstream sz_str;
    sz_str << sz;

    auto res = b
        .ns("std")
            .templ("aligned_storage", std::make_tuple(std::string{"Size"}, b.bt_int()))
                .record("astorage", cm::value{sz_str.str()})
                    .ivar("__data", b.atype(b.bt_char(), sz))
                .end()
            .end()
        .end()
    .build();

    return res.type("astorage")->cast<cm::record_type>();
}


cm::record_type * get_stdcpp_aligned_buffer_type(const cm::qual_type & type) {
    cm_builder b;
    auto sz = b.dbg().type_size(type);
    std::ostringstream sz_str;
    sz_str << sz;

    auto res = b
        .ns("std")
            .templ("aligned_storage", std::make_tuple(std::string{"Size"}, b.bt_int()))
                .record("astorage", cm::value{sz_str.str()})
                    .ivar("__data", b.atype(b.bt_char(), sz))
                .end()
            .end()
        .end()
        .ns("__gnu_cxx")
            .templ("__aligned_buffer", "T").record("buffer", type)
                .ivar("_M_storage", b.typeref("astorage"))
            .end().end()
        .end()
    .build();

    return res.type("buffer")->cast<cm::record_type>();
}



cm::record_type * get_stdcpp_pair_type(const cm::qual_type & first,
                                       const cm::qual_type & second) {
    cm_builder b;
    return b
        .ns("std")
            .templ("pair", "T1", "T2").record("pair", first.type(), second.type())
                .ivar("first", first.type())
                .ivar("second", second.type())
            .end().end()
        .end()
    .build().type("pair")->cast<cm::record_type>();
}


}
