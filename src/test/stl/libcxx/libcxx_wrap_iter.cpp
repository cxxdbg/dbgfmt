
/// \file libcxx_wrap_iter.cpp
/// Contains implementation of functions for creating types and values for the libc++ __wrap_iter.

#include "libcxx_wrap_iter.hpp"


namespace dbgfmt::test::tcm::libcxx::wrap_iter {
    cm::type_t * type(const cm::qual_type & elt_type) {
        cm_builder b;
        return b
            .ns("std").ns("__1")
                .templ("__wrap_iter", "T").record("__wrap_iter", elt_type)
                    .ivar("__i", b.ptype(elt_type))
                .end().end()
            .end().end()
        .build().type("__wrap_iter");
    }

    value make(const cm::qual_type & elt_type, const value & ptr) {
        auto val = make_val(type(elt_type));
        val["__i"] << ptr;
        return val;
    }
}
