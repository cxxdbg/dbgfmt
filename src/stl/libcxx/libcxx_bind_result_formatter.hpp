
/// \file libcxx_bind_result_formatter.hpp
/// Contains definition of formatter for result of libc++ std::bind

#pragma once

#include "../std_bind_result_formatter.hpp"
#include "libcxx_ns_utils.hpp"


namespace dbgfmt {


/// Formatter for libc++ std::bind result
class libcxx_bind_result_formatter: public std_bind_result_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "__bind");
    }

private:
    /// Returns value containing bind target
    value bind_target(const value & val) const override {
        return val["__f_"];
    }

    /// Returns value containing bound arguments
    value bound_arguments(const value & val) const override {
        return val["__bound_args_"];
    }
};


}
