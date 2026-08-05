
/// \file stdcpp_placeholder_formatter.hpp
/// Contains definition of libstdc++ placeholder formatter

#pragma once

#include "../std_placeholder_formatter.hpp"
#include <sstream>


namespace dbgfmt {


/// Formatter for libstdc++ placeholder
class stdcpp_placeholder_formatter: public std_placeholder_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return v.is_record() && v.template_name() == "std::_Placeholder";
    }
};


}
