
/// \file stdcpp_tuple.hpp
/// Contains declarations of helper functions for creating mock
/// libstdc++ tuples

#pragma once

#include "../../test2.hpp"
#include <algorithm>
#include <ranges>
#include <vector>


namespace dbgfmt::test {


cm::record_type * get_tuple_head_base_type(const cm::qual_type & t);


/// Gets existing or creates new tuple impl type for range of types
template <typename Types>
static cm::record_type * get_tuple_impl_type(const Types & types) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto std = cm.get_or_create_namespace("std");
    auto Tuple_impl = std->get_or_create_template_record("_Tuple_impl", cm::record_kind::class_, true, "Elements");

    auto Tuple_impl_inst = Tuple_impl->find_instantiation(types);
    if (!Tuple_impl_inst) {
        Tuple_impl_inst = Tuple_impl->create_instantiation(types);

        auto first_it = std::ranges::begin(types);
        assert(first_it != std::ranges::end(types) && "Range should not be empty");
        ++first_it;

        if (first_it != std::ranges::end(types)) {
            auto tail = std::ranges::subrange{first_it, std::ranges::end(types)};
            Tuple_impl_inst->add_base(get_tuple_impl_type(tail));
        }

        Tuple_impl_inst->add_base(get_tuple_head_base_type(*std::ranges::begin(types)));
        tctx.dbg().make_def_rec_layout(Tuple_impl_inst);
    }

    return Tuple_impl_inst;
}


/// Gets existing or creates new tuple record type from range of types
template <typename Types>
cm::record_type * get_stdcpp_tuple_type(const Types & types) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto std = cm.get_or_create_namespace("std");
    auto tuple = std->get_or_create_template_record("tuple", cm::record_kind::class_, true, "Elements");

    auto tuple_inst = tuple->find_instantiation(types);

    if (tuple_inst) {
        return tuple_inst;
    }

    tuple_inst = tuple->create_instantiation(types);

    if (!std::ranges::empty(types)) {
        tuple_inst->add_base(get_tuple_impl_type(types));
    }

    tctx.dbg().make_def_rec_layout(tuple_inst);
    return tuple_inst;
}


/// Gets or creates new tuple record type from initializer list of types
inline cm::record_type * get_stdcpp_tuple_type(const std::initializer_list<cm::qual_type> & types) {
    return get_stdcpp_tuple_type<std::initializer_list<cm::qual_type>>(types);
}


/// Makes head base record for libstdc++ tuple with specified value
value make_stdcpp_tuple_head_base(const value & v);


/// Makes libstdc++ tuple impl record for range of values
template <typename Values>
value make_stdcpp_tuple_impl(const Values & vals) {
    assert(!std::ranges::empty(vals) && "Range should not be empty");

    auto get_type_fn = [](auto && val) { return val_type(val); };

    auto type = get_tuple_impl_type(vals | std::ranges::views::transform(get_type_fn));
    auto rec = make_val(type);

    auto first_it = std::begin(vals);
    auto first_val = *first_it;
    auto Head_base_inst = get_tuple_head_base_type(val_type(first_val));

    size_t head_base_idx = 0;
    ++first_it;
    if (first_it != std::end(vals)) {
        auto tail = std::ranges::subrange{first_it, std::end(vals)};
        auto tail_types = tail | std::ranges::views::transform(get_type_fn);
        auto tail_impl_type = get_tuple_impl_type(tail_types);
        rec.base_at(0) << make_stdcpp_tuple_impl(tail);
        head_base_idx = 1;
    }

    rec.base_at(head_base_idx) << make_stdcpp_tuple_head_base(first_val);

    return rec;
}


/// Makes tuple record from range of values
template <typename Values>
value make_stdcpp_tuple_r(const Values & vals) {
    auto & tctx = test_context::current_context();

    std::vector<cm::qual_type> types;
    for (auto && v : vals) {
        types.push_back(val_type(v));
    }

    auto type = get_stdcpp_tuple_type(types);
    auto res = make_val(type);

    if (std::empty(vals))
        return res;

    res.base_at(0) << make_stdcpp_tuple_impl(vals);
    return res;
}


/// Makes tuple record from initializer list of values
inline value make_stdcpp_tuple_r(const std::initializer_list<value> & vals) {
    return make_stdcpp_tuple_r<std::initializer_list<value>>(vals);
}


/// Makes tuple from list of values
template <typename ... Values>
value make_stdcpp_tuple(Values && ... vals) {
    return make_stdcpp_tuple_r({vals...});
}


}


