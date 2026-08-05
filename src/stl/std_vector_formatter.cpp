
/// \file std_vector_formatter.cpp
/// Contains implementation of std_vector_formatter class.

#include "std_vector_formatter.hpp"
#include "dbgfmt/format_error.hpp"


namespace dbgfmt {



static const std::size_t max_vector_size = 100000;


bool std_vector_formatter::accepts(const value & val) const {
    if (!val.is_record()) {
        return false;
    }

    if (!accepts_type(val)) {
        return false;
    }

    value start, end, end_of_storage;
    read_vector(val, start, end, end_of_storage);
    return true;
}


bool std_vector_formatter::has_capacity() const {
    return true;
}


std::size_t std_vector_formatter::capacity(const value & val) const {
    value start, end, storage_end;
    read_vector(val, start, end, storage_end);

    std::int64_t icapacity = storage_end - start;
    if (icapacity < 0)
        throw dbgfmt::format_error("std::vector capacity is negative");

    return static_cast<std::size_t>(icapacity);
}


std::size_t std_vector_formatter::size(const value & val) const {
    value start, end, storage_end;
    read_vector(val, start, end, storage_end);
    return get_std_vector_size(start, end);
}


value std_vector_formatter::element(const value & val, std::size_t index) const {
    value start, end, storage_end;
    read_vector(val, start, end, storage_end);
    return get_std_vector_element(start, index);
}


size_t get_std_vector_size(const value & start, const value & end) {
    std::int64_t isize = end - start;
    if (isize < 0)
        throw dbgfmt::format_error("std::vector size is negative");

    std::size_t sz = static_cast<std::size_t>(isize);
    if (sz > max_vector_size)
        throw dbgfmt::format_error("std::vector is too large");

    return sz;
}


value get_std_vector_element(const value & start, size_t index) {
    return *(start + static_cast<std::int64_t>(index));
}


}
