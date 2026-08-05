

/// \file libcxx_bitset_formatter.hpp
/// Contains definition of the libcxx_bitset_formatter class.

#pragma once

#include "../std_bitset_formatter.hpp"
#include "libcxx_ns_utils.hpp"


namespace dbgfmt {


/// libc++ bitset formatter
class libcxx_bitset_formatter: public std_bitset_formatter {
public:
    /// Constructs formatter
    libcxx_bitset_formatter(): std_bitset_formatter{"__first_"} {}

    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "bitset");
    }

    /// Returns type name visible to user. Default implementation returns value type name
    std::wstring disp_type_name(const value & val) override {
        return replace_libcxx_type_name(val);
    }
};


}
