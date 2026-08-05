
/// \file stdcpp_normal_iterator.cpp
/// Contains implementations of functions for creating types and values for libstdc++ normal iterator.

#include "stdcpp_normal_iterator.hpp"


namespace dbgfmt::test {


/// Creates code model for libstdc++ normal iterator
static cm::builder_result stdcpp_normal_iterator_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("__gnu_cxx")
            .templ("__normal_iterator", "T").record("stdcpp_normal_iterator", elt_type)
                .ivar("_M_current", b.ptype(elt_type))
            .end().end()
        .end()
    .build();
}


cm::type_t * stdcpp_normal_iterator_type(const cm::qual_type & elt_type) {
    return stdcpp_normal_iterator_cm(elt_type).type("stdcpp_normal_iterator");
}


dbgfmt::value create_stdcpp_normal_iterator(const cm::qual_type & elt_type, const dbgfmt::value & ptr) {
    auto it = make_val(stdcpp_normal_iterator_type(elt_type));
    it["_M_current"] << ptr;
    return it;
}


}
