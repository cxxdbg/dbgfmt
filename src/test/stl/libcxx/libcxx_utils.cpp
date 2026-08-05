
/// \file libcxx_utils.cpp
/// Contains implementations of utility functions for testing libc++ formatters.

#include "libcxx_utils.hpp"
#include "../../cm_builder.hpp"


namespace dbgfmt::test {


cm::record_type * get_libcxx_pair_type(const cm::qual_type & first, const cm::qual_type & second) {
    cm_builder b;
    return b.ns("std").ns("__1")
        .templ("pair", "First", "Second").record("pair", first, second)
            .ivar("first", first)
            .ivar("second", second)
        .end().end()
    .end().end().build().rtype("pair");
}


value make_libcxx_pair(const value & first, const value & second) {
    auto res = make_val(get_libcxx_pair_type(val_type(first), val_type(second)));
    res["first"] << first;
    res["second"] << second;
    return res;
}


cm::record_type * get_libcxx_compressed_pair_type(const cm::qual_type & first) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("__compressed_pair", "First").record("pair", first)
                .ivar("__value_", first)
            .end().end()
        .end().end()
    .build().rtype("pair");
}


}
