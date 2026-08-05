
/// \file vector_formatter.hpp
/// Contains definition of stdcpp_vector_formatter class.

#pragma once

#include "../std_vector_formatter.hpp"
#include "../../container_iterator_formatter.hpp"


namespace dbgfmt {


/// Formatter for stdc++ vector;
class stdcpp_vector_formatter: public std_vector_formatter {
private:
    /// Returns true if formatter accepts type
    virtual bool accepts_type(const value & val) const;
    
    /// Reads vector start, end, and end of storage fields
    virtual void read_vector(const value & val,
                             value & start,
                             value & end,
                             value & end_of_storage) const;
};


/// Formatter for stdc++ vector iterator
class stdcpp_vector_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    virtual bool accepts(const value & v) const;

private:
    /// Returns pointer to value pointed by iterator
    virtual value get_value_pointer(const value & it) const;
};


/// Returns size of libstdc++ vector
size_t get_stdcpp_vector_size(const value & val);

/// Return value of libstdc++ vector element at specified index
value get_stdcpp_vector_element(const value & val, size_t index);


}


