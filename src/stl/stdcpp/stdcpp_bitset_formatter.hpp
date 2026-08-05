
/// \file stdcpp_bitset_formatter.hpp
/// Contains definition of the stdcpp_bitset_formatter class.

#pragma once

#include "../std_bitset_formatter.hpp"


namespace dbgfmt {


/// libstdc++ bitset formatter
class stdcpp_bitset_formatter: public std_bitset_formatter {
public:
    /// Constructs formatter
    stdcpp_bitset_formatter(): std_bitset_formatter{"_M_w"} {}

    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return v.is_record() && v.template_name() == "std::bitset";
    }
};


}


