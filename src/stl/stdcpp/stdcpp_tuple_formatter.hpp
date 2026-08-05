
/// \file stdcpp_tuple_formatter.hpp
/// Contains definition of the stdcpp_tuple_formatter class.

#pragma once

#include "../std_tuple_formatter.hpp"


namespace dbgfmt {


/// libstdc++ tuple formatter
class stdcpp_tuple_formatter: public std_tuple_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

private:
    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


/// Retursn tuple size
size_t stdcpp_tuple_size(const value & val);


/// Returns tuple element with specified index
value stdcpp_tuple_element(const value & val, std::size_t index);


}


