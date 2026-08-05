
/// \file libcxx_string_view_formatter.hpp
/// Contains definition of the libcxx_string_view_formatter class.

#pragma once

#include "libcxx_ns_utils.hpp"
#include "../std_string_view_formatter.hpp"


namespace dbgfmt {


/// Formatter for libc++ string_view class
class libcxx_string_view_formatter: public std_string_view_formatter {
public:
    /// Constructs formatter
    libcxx_string_view_formatter():
        std_string_view_formatter{"__data", "__size"} {}

    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "basic_string_view");
    }

private:
    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override {
        return replace_libcxx_type_name(val);
    }
};


}
