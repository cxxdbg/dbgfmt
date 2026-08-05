
/// \file libcxx_deque_formatter.cpp
/// Contains implementation of the libc++ deque formatter.

#include "libcxx_deque_formatter.hpp"
#include "libcxx_ns_utils.hpp"
#include "dbgfmt/context.hpp"


namespace dbgfmt {


/// Calculates deque block size from type size
static size_t calc_deque_block_size(std::size_t type_size) {
    return type_size < 256 ?
           4096 / type_size :
           16;
}


bool libcxx_deque_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "deque");
}


size_t libcxx_deque_formatter::size(const value & val) const {
    return static_cast<size_t>(val["__size_"]["__value_"].as_uint64());
}


value libcxx_deque_formatter::element(const value & val, size_t index) const {
    auto elt_type = ctx().find_type(val.type().template_param_name(0));
    auto elt_size = elt_type.size();
    auto block_size = calc_deque_block_size(elt_size);

    auto real_idx = index + val["__start_"].as_uint64();
    auto block_idx = real_idx / block_size;
    auto elt_idx_in_block = real_idx % block_size;

    auto block = *(val["__map_"]["__begin_"] + block_idx);
    return *(block + elt_idx_in_block);
}


std::wstring libcxx_deque_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}


bool libcxx_deque_iterator_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "__deque_iterator");
}


value libcxx_deque_iterator_formatter::get_value_pointer(const value & it) const {
    return it["__ptr_"];
}



}
