
/// \file vector_formatter.hpp
/// Contains definition of stdcpp_vector_formatter class.

#pragma once

#include "../std_vector_formatter.hpp"
#include "../../container_iterator_formatter.hpp"


namespace dbgfmt {


/// Formatter for stdc++ vector;
class libcxx_vector_formatter: public std_vector_formatter {
private:
    /// Returns true if formatter accepts type
    bool accepts_type(const value & val) const override;

    /// Reads vector start, end, and end of storage fields
    void read_vector(const value & val,
                     value & start,
                     value & end,
                     value & end_of_storage) const override;

    /// Returns type name that should be displayed for user.
    std::wstring disp_type_name(const value & val) const override;
};


/// Formatter for libc++ vector iterator
class libcxx_vector_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

private:
    /// Returns pointer to value pointed by iterator
    value get_value_pointer(const value & it) const override;
};


/// Returns size of libc++ vector
size_t get_libcxx_vector_size(const value & v);

/// Returns element of libc++ vector with specified index
value get_libcxx_vector_element(const value & v, size_t index);


}
