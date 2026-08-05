
/// \file libcxx_vector.cpp
/// Contains implementation of functions for creating types and values for libc++ vector.

#include "libcxx_vector.hpp"
#include "../std_vector_utils.hpp"


namespace dbgfmt::test::tcm::libcxx::vector {


cm::type_t * type(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .record("compressed_pair", "compressed_pair")
                .ivar("__value_", b.ptype(elt_type))
            .end()

            .templ("vector", "T").record("libcxx_vector", elt_type)
                .ivar("__begin_", b.ptype(elt_type))
                .ivar("__end_", b.ptype(elt_type))
                .ivar("__end_cap_", b.typeref("compressed_pair"))
            .end().end()
        .end().end()
    .build().type("libcxx_vector");
}


value make(const cm::qual_type & elt_type, const value & begin, const value & end) {
    auto v = make_val(type(elt_type));
    v["__begin_"] << begin;
    v["__end_"] << end;
    v["__end_cap_"]["__value_"] << end;
    return v;
}


value make(const cm::qual_type & elt_type, const std::vector<value> & vals) {
    auto [begin, end] = make_vector_begin_end_tuple(elt_type, vals);
    return make(elt_type, begin, end);
}


}
