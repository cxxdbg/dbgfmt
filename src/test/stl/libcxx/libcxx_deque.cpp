
/// \file libcxx_deque.cpp
/// Contains implementations of utility functions for testing libc++ deque.

#include "libcxx_deque.hpp"


namespace dbgfmt::test {


cm::qual_type get_libcxx_deque_type(const cm::qual_type & elt_type) {
    return libcxx_deque_cm(elt_type).type("deque");
}


/// Calculates deque block size from element type size
static size_t calc_block_size(size_t type_size) {
    return type_size < 256 ?
           static_cast<size_t>(4096 / type_size) :
           16;
}


/// Calculates deque block size from element type
static size_t calc_block_size(const cm::const_qual_type & type) {
    auto & tctx = test_context::current_context();
    return calc_block_size(tctx.dbg().type_size(type));
}


value make_empty_libcxx_deque(const cm::qual_type & elt_type) {
    auto dcm = libcxx_deque_cm(elt_type);
    auto deque = make_val(dcm.type("deque"));

    deque["__map_"]["__first_"] << 0;
    deque["__map_"]["__begin_"] << 0;
    deque["__map_"]["__end_"] << 0;
    deque["__map_"]["__end_cap_"]["__value_"] << 0;
    deque["__start_"] << 0;
    deque["__size_"]["__value_"] << 0;

    return deque;
}


value make_libcxx_deque(const cm::qual_type & elt_type, const std::vector<value> & vals, size_t first) {
    auto block_size = calc_block_size(elt_type);
    auto last = vals.empty() ? first : first + vals.size() - 1;
    auto last_block = last / block_size;
    auto n_blocks = last_block + 1;

    // creating deque blocks
    std::vector<value> blocks;
    for (size_t i = 0; i < n_blocks; ++i) {
        blocks.push_back(make_arr(elt_type, block_size));
    }

    // filling blocks with values
    for (size_t i = 0, e = vals.size(); i < e; ++i) {
        auto elt_idx = first + i;
        auto block_idx = elt_idx / block_size;
        auto elt_idx_in_block = elt_idx % block_size;
        blocks[block_idx][elt_idx_in_block] << vals[i];
    }

    // creating deque map
    auto elt_ptr_type = test_context::current_context().cm().get_or_create_ptr_type(elt_type);
    auto map = make_arr(elt_ptr_type, n_blocks);
    for (size_t i = 0; i < n_blocks; ++i) {
        map[i] << blocks[i].ptr();
    }

    auto deque = make_val(get_libcxx_deque_type(elt_type));

    deque["__map_"]["__first_"] << map.ptr();
    deque["__map_"]["__begin_"] << map.ptr();
    deque["__map_"]["__end_"] << (map.ptr() + n_blocks);
    deque["__map_"]["__end_cap_"]["__value_"] << (map.ptr() + n_blocks);
    deque["__start_"] << first;
    deque["__size_"]["__value_"] << vals.size();

    return deque;
}


value make_libcxx_deque_iterator(const value & val) {
    auto it_type = libcxx_deque_cm(val_type(val)).type("deque_iterator");
    auto it = make_val(it_type);
    it["__ptr_"] << val.ptr();
    return it;
}


}
