
/// \file stdcpp_deque_formatter.cpp
/// Contains implementation of stdc++ deque formatter classes.

#include "stdcpp_deque_formatter.hpp"
#include "dbgfmt/format_error.hpp"
#include <cassert>


namespace dbgfmt {


/// Calculates deque node size from type size
static std::size_t calc_deque_node_size(std::size_t type_size) {
    return type_size < 512 ?
           512 / type_size :
           type_size;
}


bool stdcpp_deque_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::deque";
}


std::size_t stdcpp_deque_formatter::size(const value & val) const {

    std::size_t type_size = (*val["_M_impl"]["_M_start"]["_M_cur"]).size();
    std::size_t node_size = calc_deque_node_size(type_size);

    std::size_t sz = 0;
    value node_ptr = val["_M_impl"]["_M_start"]["_M_node"];
    value cur_ptr = val["_M_impl"]["_M_start"]["_M_cur"];
    value cur_end_ptr = val["_M_impl"]["_M_start"]["_M_last"];
    value finish_ptr = val["_M_impl"]["_M_finish"]["_M_cur"];
    std::uint64_t max_size = val["_M_impl"]["_M_map_size"].as_uint64() * node_size;

    if (max_size > max_container_size)
        throw format_error("can't determine deque size");

    while (cur_ptr.as_uint64() != finish_ptr.as_uint64()) {
        ++sz;

        // checking for corrupted deque. Deque size can't be larger than
        // map size * node size
        if (sz > max_size)
            throw format_error("can't determine deque size");

        cur_ptr = cur_ptr + 1;

        if (cur_ptr.as_uint64() == finish_ptr.as_uint64())
            break;

        if (cur_ptr.as_uint64() == cur_end_ptr.as_uint64()) {
            node_ptr = node_ptr + 1;

            cur_ptr = *node_ptr;
            cur_end_ptr = cur_ptr + node_size;
        }
    }

    return sz;
}


value stdcpp_deque_formatter::element(const value & val, std::size_t index) const {
    assert(index < size(val) && "Invalid element index");

    std::size_t type_size = (*val["_M_impl"]["_M_start"]["_M_cur"]).size();
    std::size_t node_size = calc_deque_node_size(type_size);

    std::size_t ind = 0;
    value node_ptr = val["_M_impl"]["_M_start"]["_M_node"];
    value cur_ptr = val["_M_impl"]["_M_start"]["_M_cur"];
    value cur_end_ptr = val["_M_impl"]["_M_start"]["_M_last"];
    value finish_ptr = val["_M_impl"]["_M_finish"]["_M_cur"];

    while (cur_ptr.as_uint64() != finish_ptr.as_uint64()) {

        if (ind == index)
            return *cur_ptr;

        ++ind;

        cur_ptr = cur_ptr + 1;

        if (cur_ptr.as_uint64() == finish_ptr.as_uint64())
            break;

        if (cur_ptr.as_uint64() == cur_end_ptr.as_uint64()) {
            node_ptr = node_ptr + 1;

            cur_ptr = *node_ptr;
            cur_end_ptr = cur_ptr + node_size;
        }
    }

    throw format_error("Can't get deque element");
}



bool stdcpp_deque_iterator_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::_Deque_iterator";
}


value stdcpp_deque_iterator_formatter::get_value_pointer(const value & it) const {
    return it["_M_cur"];
}


}
