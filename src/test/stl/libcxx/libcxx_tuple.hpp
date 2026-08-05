
/// \file libcxx_tuple.hpp
/// Contains implementations of utility functions fo testing libc++ tuple.

#pragma once

#include "libcxx_is_empty.hpp"
#include "../../test2.hpp"
#include <algorithm>
#include <ranges>


namespace dbgfmt::test {


/// Returns std::tuple type for specfied range of element types
template <typename Elements>
cm::record_type * libcxx_tuple_type_r(const Elements & elts) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto & dbg = tctx.dbg();

    // creating std namespace
    auto std_ns = cm.get_or_create_namespace("std")->get_or_create_namespace("__1");

    // getting or creating tuple template
    auto tuple_templ = std_ns->get_or_create_template_record("tuple", cm::record_kind::class_, true, "Elements");

    // looking for existing tuple instance type
    auto tuple_type = tuple_templ->find_instantiation(elts);
    if (tuple_type) {
        return tuple_type;
    }

    // creating typle type
    tuple_type = tuple_templ->create_instantiation(elts);

    // creating tuple impl type
    auto tuple_impl_templ = std_ns->get_or_create_template_record("__tuple_impl", cm::record_kind::class_, true, "Elements");
    auto tuple_impl_type = tuple_impl_templ->create_instantiation(elts);

    // creating template for tuple leaf classes
    auto tuple_leaf_templ = std_ns->get_or_create_template_record("__tuple_leaf",
                                                            cm::record_kind::class_,
                                                            false,
                                                            std::make_tuple("N", cm.bt_int()),
                                                            "T",
                                                            std::make_tuple("B", cm.bt_bool()));

    // creating child types for each element and adding them as base classes for tuple impl type
    size_t idx = 0;
    for (auto && elt : elts) {
        auto tuple_leaf = tuple_leaf_templ->create_instantiation(cm::value{std::to_string(idx)},
                                                                 elt,
                                                                 cm::value{"false"});
        
        if (libcxx_is_empty(elt)) {
            tuple_leaf->add_base(elt.type());
        } else {
            tuple_leaf->create_field("__value_", elt);
        }

        dbg.make_def_rec_layout(tuple_leaf);

        tuple_impl_type->add_base(tuple_leaf);
        ++idx;
    }

    dbg.make_def_rec_layout(tuple_impl_type);

    // adding __base_ field into tuple type
    tuple_type->create_field("__base_", tuple_impl_type);

    dbg.make_def_rec_layout(tuple_type);

    return tuple_type;
}


/// Makes libc++ tuple value for range of element values
template <typename Values>
value make_libcxx_tuple_r(const Values & vals) {
    // creating tuple type
    auto fn = [](auto && val) { return val_type(val); };
    auto types = vals | std::ranges::views::transform(fn);
    auto tuple_type = libcxx_tuple_type_r(types);

    auto tuple = make_val(tuple_type);

    // filling values for tuple elements
    size_t idx = 0;
    for (auto && val : vals) {
        auto base = tuple["__base_"].base_at(idx);
        if (!libcxx_is_empty(val_type(val))) {
            base["__value_"] << val;
        }
        ++idx;
    }

    return tuple;
}


/// Makes libc++ tuple from initializer list of values
inline value make_libcxx_tuple_r(const std::initializer_list<value> & vals) {
    return make_libcxx_tuple_r<std::initializer_list<value>>(vals);
}


/// Returns libc++ tuple value from list of element values
template <typename ... Values>
value make_libcxx_tuple(Values && ... vals) {
    return make_libcxx_tuple_r({vals...});
}


inline value make_libcxx_tuple(const std::vector<value> & vals) {
    return make_libcxx_tuple_r(vals);
}


}
