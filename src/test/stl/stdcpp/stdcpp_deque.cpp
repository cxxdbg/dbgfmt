
/// \file stdcpp_deque.cpp
/// Contains implementation of the stdcpp_deque_record class and related classes.

#include "stdcpp_deque.hpp"
#include <cassert>


namespace dbgfmt::test {


/// Calculates deque node size from element type size
static std::size_t calc_node_size(std::size_t type_size) {
    return type_size < 512 ?
           static_cast<std::size_t>(512 / type_size) :
           type_size;
}


/// Calculates deque node size from element type
static std::size_t calc_node_size(const cm::const_qual_type & type) {
    auto & tctx = test_context::current_context();
    return calc_node_size(tctx.dbg().type_size(type));
}


cm::record_type * get_stdcpp_deque_iterator_type(const cm::qual_type & elt_type) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    // creating pointer to element type
    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);

    // creating pointer to pointer to element type
    auto elt_ptr_ptr_type = cm.get_or_create_ptr_type(elt_ptr_type);

    // creating std::_Deque_iterator template
    auto std = cm.get_or_create_namespace("std");
    auto iter_templ = std->get_or_create_template_record("_Deque_iterator", cm::record_kind::class_, false, "T");

    // creating _Deque_iterator instance if does not exist
    auto iter_type = iter_templ->find_instantiation(elt_type);
    if (!iter_type) {
        iter_type = iter_templ->create_instantiation(elt_type);
        iter_type->create_field("_M_cur", elt_ptr_type);
        iter_type->create_field("_M_first", elt_ptr_type);
        iter_type->create_field("_M_last", elt_ptr_type);
        iter_type->create_field("_M_node", elt_ptr_ptr_type);
        tctx.dbg().make_def_rec_layout(iter_type);
    }

    return iter_type;
}


cm::record_type * get_stdcpp_deque_type(const cm::qual_type & elt_type) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto std = cm.get_or_create_namespace("std");

    // getting / creating std::deque template
    auto deque_templ = std->get_or_create_template_record("deque", cm::record_kind::class_, false, "T");

    // creating std::deque instance if not created
    auto deque_inst = deque_templ->find_instantiation(elt_type);
    if (!deque_inst) {
        // creating pointer to element type
        auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);

        // creating pointer to pointer to element type
        auto elt_ptr_ptr_type = cm.get_or_create_ptr_type(elt_ptr_type);

        // getting / creating std::_Deque_impl template
        auto deque_impl_templ = std->get_or_create_template_record("_Deque_impl", cm::record_kind::class_, false, "T");

        // creating _Deque_impl instance if not created
        auto deque_impl_inst = deque_impl_templ->find_instantiation(elt_type);
        if (!deque_impl_inst) {
            auto it_type = get_stdcpp_deque_iterator_type(elt_type);
            deque_impl_inst = deque_impl_templ->create_instantiation(elt_type);
            deque_impl_inst->create_field("_M_map", elt_ptr_ptr_type);
            deque_impl_inst->create_field("_M_map_size", cm.bt_unsigned_long());
            deque_impl_inst->create_field("_M_start", it_type);
            deque_impl_inst->create_field("_M_finish", it_type);
            tctx.dbg().make_def_rec_layout(deque_impl_inst);
        }

        deque_inst = deque_templ->create_instantiation(elt_type);
        deque_inst->create_field("_M_impl", deque_impl_inst);
        tctx.dbg().make_def_rec_layout(deque_inst);
    }

    return deque_inst;
}


value make_empty_stdcpp_deque(const cm::qual_type & elt_type, size_t node_count) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto type = get_stdcpp_deque_type(elt_type);
    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);
    auto elt_ptr_ptr_type = cm.get_or_create_ptr_type(elt_ptr_type);

    // creating deque nodes
    std::vector<value> nodes;
    auto node_sz = calc_node_size(cm::const_qual_type{elt_type});

    for (size_t i = 0; i < node_count; ++i) {
        nodes.push_back(make_arr(elt_type.type(), node_sz));
    }

    // creating node map
    auto node_map = make_arr(elt_ptr_type, node_count);
    for (size_t i = 0; i < node_count; ++i) {
        node_map[i] << nodes[i].ptr();
    }

    // creating deque

    auto deque = make_val(type);
    auto impl = deque["_M_impl"];
    impl["_M_map"] << node_map.ptr();
    impl["_M_map_size"] << make_ulong(node_count);

    auto start = impl["_M_start"];
    start["_M_cur"] << nodes[0].ptr();
    start["_M_first"] << nodes[0].ptr();
    start["_M_last"] << nodes[0].ptr();
    start["_M_node"] << make_null(elt_ptr_ptr_type);

    auto finish = impl["_M_finish"];
    finish["_M_cur"] << nodes[0].ptr();
    finish["_M_first"] << nodes[0].ptr();
    finish["_M_last"] << nodes[0].ptr();
    finish["_M_node"] << make_null(elt_ptr_ptr_type);

    return deque;
}


value make_stdcpp_deque_iterator(const cm::qual_type & elt_type,
                                 const value & cur,
                                 const value & first,
                                 const value & last,
                                 const value & node) {
    auto rec = make_val(get_stdcpp_deque_iterator_type(elt_type));
    rec["_M_cur"] << cur;
    rec["_M_first"] << first;
    rec["_M_last"] << last;
    rec["_M_node"] << node;
    return rec;
}


value make_stdcpp_deque_iterator(const cm::qual_type & elt_type,
                                 uint64_t cur,
                                 uint64_t first,
                                 uint64_t last,
                                 uint64_t node) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);
    auto elt_ptr_ptr_type = cm.get_or_create_ptr_type(elt_ptr_type);

    return make_stdcpp_deque_iterator(elt_type,
                                      make_ptr(elt_ptr_type, cur),
                                      make_ptr(elt_ptr_type, first),
                                      make_ptr(elt_ptr_type, last),
                                      make_ptr(elt_ptr_ptr_type, node));
}


value make_stdcpp_deque_invalid_node(const cm::qual_type & elt_type) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto node_size = calc_node_size(cm::const_qual_type{elt_type});
    auto arr_type = cm.get_or_create_arr_type(elt_type.type(), node_size);
    return make_err(arr_type);
}


value make_stdcpp_deque(const cm::qual_type & elt_type,
                        const value & map,
                        const value & map_size,
                        const value & start,
                        const value & finish) {
    auto deque = make_val(get_stdcpp_deque_type(elt_type));
    auto impl = deque["_M_impl"];
    impl["_M_map"] << map;
    impl["_M_map_size"] << map_size;
    impl["_M_start"] << start;
    impl["_M_finish"] << finish;
    return deque;
}


value make_stdcpp_deque(const cm::qual_type & elt_type,
                        const std::vector<value> & vals,
                        size_t start_elt) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto type = get_stdcpp_deque_type(elt_type);
    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);
    auto elt_ptr_ptr_type = cm.get_or_create_ptr_type(elt_ptr_type);

    std::vector<value> nodes;
    auto node_size = calc_node_size(cm::const_qual_type{elt_type});
    auto elt_size = tctx.dbg().type_size(elt_type);
    auto node_byte_size = node_size * elt_size;

    // creating emty nodes
    auto num_empty_nodes = start_elt / node_size;
    for (std::size_t i = 0; i < num_empty_nodes; ++i) {
        nodes.push_back(make_arr(elt_type, node_size));
    }

    // creating first node
    auto node = make_arr(elt_type, node_size);
    std::size_t first_node_index = num_empty_nodes;
    std::size_t first_node_start = start_elt % node_size;
    std::size_t cur_node_size = first_node_start;
    std::size_t cur_index = 0;

    // creating nodes
    while (cur_index < vals.size()) {

        if (cur_node_size == node_size) {
            // adding new node
            nodes.push_back(node);
            cur_node_size = 0;
        }

        node[cur_node_size] << vals[cur_index];
        ++cur_node_size;

        ++cur_index;
    }

    nodes.push_back(node);

    // creating node map
    auto node_map = make_arr(elt_ptr_type, nodes.size());
    for (size_t i = 0, e = nodes.size(); i < e; ++i) {
        node_map[i] << nodes[i].ptr();
    }

    // creating deque

    auto map = node_map.ptr();
    auto map_size = make_ulong(nodes.size());

    auto s_cur = nodes[first_node_index][first_node_start].address();
    auto s_first = nodes[first_node_index].address();
    auto s_last = nodes[first_node_index].address() + node_byte_size;
    auto s_node = node_map[first_node_index].address();
    auto start = make_stdcpp_deque_iterator(elt_type, s_cur, s_first, s_last, s_node);

    auto f_cur = nodes.back()[cur_node_size].address();
    auto f_first = nodes.back().address();
    auto f_last = nodes.back().address() + node_byte_size;
    auto f_node = node_map[first_node_index].address();
    auto finish = make_stdcpp_deque_iterator(elt_type, f_cur, f_first, f_last, f_node);

    return make_stdcpp_deque(elt_type, map, map_size, start, finish);
}


}
