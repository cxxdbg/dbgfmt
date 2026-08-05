
/// \file libcxx_tuple_formatter.hpp
/// Contains definition of libc++ tuple formatter.

#pragma once

#include "libcxx_ns_utils.hpp"
#include "../std_tuple_formatter.hpp"


namespace dbgfmt {


/// Formatter for libc++ tuple
class libcxx_tuple_formatter: public std_tuple_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "tuple");
    }

private:
    /// Returns container size
    std::size_t size(const value & val) const override {
        if (!val.has_field("__base_")) {
            return 0;
        }

        return val["__base_"].bases_size();
    }

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override {
        assert(index < size(val) && "invalid element index");
        auto elt = val["__base_"].base_at(index);
        if (elt.has_field("__value_")) {
            return elt["__value_"];
        } else {
            return elt.base_at(0);
        }
    }

    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override {
        return replace_libcxx_type_name(val);
    }
};


}
