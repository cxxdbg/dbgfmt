
/// \file std_vector_formatter.hpp
/// Contains definition of std_vector_formatter class.

#pragma once

#include "../container_formatter.hpp"


namespace dbgfmt {


/// \class std_vector_formatter
/// Container formatter implementation for formatting STL vectors
class std_vector_formatter: public container_formatter {
public:
    /// Returs true if formatter accepts specified value. Calls
    /// read_vector function to check that all required fields exist
    bool accepts(const value & val) const override;

private:
    /// Returns true if formatter accepts type
    virtual bool accepts_type(const value & val) const = 0;

    /// Reads vector start, end, and end of storage fields
    virtual void read_vector(const value & val,
                             value & start,
                             value & end,
                             value & end_of_storage) const = 0;

    /// Returns true
    bool has_capacity() const override;

    /// Returns vector capacity
    std::size_t capacity(const value & val) const override;

    /// Returns vector size
    std::size_t size(const value & val) const override;

    /// Returns value object for vector element with specified index
    value element(const value & val, std::size_t index) const override;
};


/// Returns vector size for specified start and end pointers. Check for negative
/// size and max vector size. Throws exception if size is bad.
size_t get_std_vector_size(const value & start, const value & end);


/// Returns vector element for specified start pointer value and index
value get_std_vector_element(const value & start, size_t index);


}


