
/// \file stdcpp_vector.cpp
/// Contains implementation of functions for building types and values for libstdc++ vector.

#include "stdcpp_vector.hpp"
#include "../std_vector_utils.hpp"


namespace dbgfmt::test {


cm::builder_result stdcpp_vector_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std")
            .templ("vector_impl", "T").record("impl", elt_type)
                .ivar("_M_start", b.ptype(elt_type))
                .ivar("_M_finish", b.ptype(elt_type))
                .ivar("_M_end_of_storage", b.ptype(elt_type))
            .end().end()

            .templ("vector", "T").record("stdcpp_vector", elt_type)
                .ivar("_M_impl", b.typeref("impl"))
            .end().end()
        .end()
    .build();
}


cm::type_t * get_stdcpp_vector_type(const cm::qual_type & elt_type) {
    return stdcpp_vector_cm(elt_type).type("stdcpp_vector");
}


value make_stdcpp_vector_begin_end(const cm::qual_type & elt_type, const value & begin, const value & end) {
    auto vcm = stdcpp_vector_cm(elt_type);

    auto v = make_val(vcm.rtype("stdcpp_vector"));
    v["_M_impl"]["_M_start"] << begin;
    v["_M_impl"]["_M_finish"] << end;
    v["_M_impl"]["_M_end_of_storage"] << end;
    return v;
}


value make_stdcpp_vector(const cm::qual_type & elt_type, const std::vector<value> & vals) {
    auto [begin, end] = make_vector_begin_end_tuple(elt_type, vals);
    return make_stdcpp_vector_begin_end(elt_type, begin, end);
}


}
