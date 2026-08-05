
/// \file vector_formatter.cpp
/// Contains implementation of vector_formatter class.

#include "stdcpp_vector_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/format_error.hpp"
#include <cassert>
#include <sstream>


namespace dbgfmt {


/// Returns libstdc++ vector start pointer value
static value get_stdcpp_vector_start(const value & val) {
    return val["_M_impl"]["_M_start"];
}


/// Returns libstdc++ vector end pointer value
static value get_stdcpp_vector_end(const value & val) {
    return val["_M_impl"]["_M_finish"];
}


bool stdcpp_vector_formatter::accepts_type(const value & val) const {
    return val.template_name() == "std::vector";
}


void stdcpp_vector_formatter::read_vector(const value & val,
                                          value & start,
                                          value & end,
                                          value & end_of_storage) const {
    start = get_stdcpp_vector_start(val);
    end = get_stdcpp_vector_end(val);
    end_of_storage = val["_M_impl"]["_M_end_of_storage"];
}


bool stdcpp_vector_iterator_formatter::accepts(const value & v) const {
    if (!v.is_record())
        return false;

    if (v.template_name() != "__gnu_cxx::__normal_iterator")
        return false;

    // TODO: should we check for vector template parameter?
    return true;
}


value stdcpp_vector_iterator_formatter::get_value_pointer(const value & it) const {
    return it["_M_current"];
}


size_t get_stdcpp_vector_size(const value & val) {
    return get_std_vector_size(get_stdcpp_vector_start(val), get_stdcpp_vector_end(val));
}


value get_stdcpp_vector_element(const value & val, size_t index) {
    return get_std_vector_element(get_stdcpp_vector_start(val), index);
}


}
