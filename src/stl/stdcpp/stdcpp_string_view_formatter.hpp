
/// \file stdcpp_string_view_formatter.hpp
/// Contains definition of the stdcpp_string_view_formatter class.

#pragma once

#include "../std_string_view_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ string_view class
class stdcpp_string_view_formatter: public std_string_view_formatter {
public:
    /// Constructs formatter
    stdcpp_string_view_formatter():
        std_string_view_formatter{"_M_str", "_M_len"} {}

    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return v.is_record() && v.template_name() == "std::basic_string_view";
    }
};


}
