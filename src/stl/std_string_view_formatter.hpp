
/// \file std_string_view_formatter.hpp
/// Contains definition of the std_string_view_formatter class.

#pragma once

#include "std_string_formatter.hpp"


namespace dbgfmt {


/// Base class for string view formatters. Contains common logic
class std_string_view_formatter: public std_string_formatter {
public:
    /// Constructs string view formatter with specified names of data
    /// and size fields
    std_string_view_formatter(std::string d_fld_name, std::string sz_fld_name):
        data_field_name_{std::move(d_fld_name)},
        size_field_name_{std::move(sz_fld_name)} {}

    /// Always returns false
    bool has_capacity() const override { return false; }

protected:
    /// Returns container size
    std::size_t size(const value & val) const override {
        return static_cast<size_t>(val[size_field_name_].as_uint64());
    }

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override {
        return *(val[data_field_name_] + index);
    }

    std::string data_field_name_;       ///< Name of data field
    std::string size_field_name_;       ///< Name of size field
};


}
