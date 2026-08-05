
/// \file libcxx_placeholder_formatter.hpp
/// Contains definition of libc++ placeholder formatter

#pragma once

#include "libcxx_ns_utils.hpp"
#include "../std_placeholder_formatter.hpp"
#include <sstream>


namespace dbgfmt {


/// Formatter for libc++ placeholder
class libcxx_placeholder_formatter: public std_placeholder_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "placeholders::__ph");
    }
};


}
