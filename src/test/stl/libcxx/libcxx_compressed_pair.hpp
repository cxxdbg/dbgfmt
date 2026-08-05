
/// \file libcxx_compressed_pair.hpp
/// Contains definition of utility functions for creating libc++ compressed pair.

#pragma once

#include "libcxx_is_empty.hpp"
#include "../../test2.hpp"


namespace dbgfmt::test {


/// Gets type of libc++ compressed pair element for specified type.
inline cm::type_t * libcxx_compressed_pair_elem_type(const cm::qual_type & type, int idx) {
    auto & cm = test_context::current_context().cm();
    auto std_ns = cm.get_or_create_namespace("std")->get_or_create_namespace("__1");

    // getting or creating template for compressed pair element
    auto cpair_elem = std_ns->get_or_create_template_record("__compressed_pair_elem",
                                                      cm::record_kind::class_,
                                                      false,
                                                      "T",
                                                      std::make_tuple("Index", cm.bt_int()),
                                                      std::make_tuple("Bool", cm.bt_bool()));

    // trying get existing template instantiation
    auto cpair_elem_type = cpair_elem->find_instantiation(type, cm::value{idx}, cm::value{"false"});
    if (cpair_elem_type) {
        return cpair_elem_type;
    }

    // creating new template instantiation
    cpair_elem_type = cpair_elem->create_instantiation(type, cm::value{idx}, cm::value{"false"});

    if (libcxx_is_empty(type)) {
        // compressed pair element should be inherited from empty type
        auto rtype = type.cast<cm::record_type>();
        assert(rtype && "empty type must be a record type");
        cpair_elem_type->add_base(rtype.type());
    } else {
        // compressed pair element should has __value_ field for non empty type
        cpair_elem_type->create_field("__value_", type);
    }

    test_context::current_context().dbg().make_def_rec_layout(cpair_elem_type);

    return cpair_elem_type;
}


/// Gets libc++ compressed pair type for specified first and second types
inline cm::type_t * libcxx_compressed_pair_type(const cm::qual_type & first, const cm::qual_type & second) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("__compressed_pair", "First", "Second").record("compressed_pair", first, second)
                .base(libcxx_compressed_pair_elem_type(first, 0))
                .base(libcxx_compressed_pair_elem_type(second, 1))
            .end().end()
        .end().end()
    .build().type("compressed_pair");
}


/// Makes libc++ compressed pair element value for specified type
inline value make_libcxx_compressed_pair_elem(const value & val, int idx) {
    auto elem_type = libcxx_compressed_pair_elem_type(val_type(val), idx);
    auto elem_val = make_val(elem_type);

    if (libcxx_is_empty(val_type(val))) {
        elem_val.base_at(0) << val;
    } else {
        elem_val["__value_"] << val;
    }

    return elem_val;
}


/// Makes libc++ compressed pair value for specified first and second values
inline value make_libcxx_compressed_pair(const value & first, const value & second) {
    auto pair_type = libcxx_compressed_pair_type(val_type(first), val_type(second));
    auto pair = make_val(pair_type);
    pair.base_at(0) << make_libcxx_compressed_pair_elem(first, 0);
    pair.base_at(1) << make_libcxx_compressed_pair_elem(second, 1);
    return pair;
}


}
