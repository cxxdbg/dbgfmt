
/// \file stdcpp_bind_result_formatter.hpp
/// Contains definition of formatter for libstdc++ std::bind result

#pragma once

#include "../std_bind_result_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ std::bind result
class stdcpp_bind_result_formatter: public std_bind_result_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override {
        return v.is_record() && v.template_name() == "std::_Bind";
    }

private:
    /// Returns value containing bind target
    value bind_target(const value & val) const override {
        auto target = val["_M_f"];
        if (target.is_record() && target.template_name() == "std::_Mem_fn") {
            // wrapper around pointer to member function
            target = target["_M_pmf"];
        }

        return target;
    }

    /// Returns value containing bound arguments
    value bound_arguments(const value & val) const override {
        return val["_M_bound_args"];
    }
};


}
