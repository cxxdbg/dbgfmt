
/// \file libcxx_vector_formatter.cpp
/// Contains implementation of libc++ vector formatter.

#include "libcxx_ns_utils.hpp"
#include "libcxx_vector_formatter.hpp"
#include "../std_vector_formatter.hpp"


namespace dbgfmt {


/// Returns libcxx vector start pointer
static value get_libcxx_vector_start(const value & val) {
    return val["__begin_"];
}


/// Returns libcxx vector end pointer
static value get_libcxx_vector_end(const value & val) {
    return val["__end_"];
}


bool libcxx_vector_formatter::accepts_type(const value & val) const {
    return is_libcxx_record(val, "vector");
}


void libcxx_vector_formatter::read_vector(const value & val,
                                          value & start,
                                          value & end,
                                          value & end_of_storage) const {
    start = get_libcxx_vector_start(val);
    end = get_libcxx_vector_end(val);
    end_of_storage = val["__end_cap_"]["__value_"];
}


std::wstring libcxx_vector_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}


bool libcxx_vector_iterator_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "__wrap_iter");
}


value libcxx_vector_iterator_formatter::get_value_pointer(const value & it) const {
    return it["__i"];
}



size_t get_libcxx_vector_size(const value & v) {
    return get_std_vector_size(get_libcxx_vector_start(v), get_libcxx_vector_end(v));
}


value get_libcxx_vector_element(const value & v, size_t index) {
    return get_std_vector_element(get_libcxx_vector_start(v), index);
}


}
