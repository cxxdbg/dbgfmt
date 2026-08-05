

/// \file mem_fun_formatter.hpp
/// Contains definition of the mem_fun_formatter class

#pragma once

#include "proxy_fmt_result.hpp"
#include "value_conv_formatter.hpp"


namespace dbgfmt {


/// Formatter for member function in old representation
class mem_fun_formatter: public value_conv_formatter {
public:
    /// Returns true if formatter accepts specified value
    bool accepts(const value & val) const override {
        return val.type().is_mem_ptr() && val.type().mem_ptr_mem_type().is_function();
    }

private:
    /// Converts value for formatting
    value convert(const value & val) const override {
        auto fptr_type = val.type().mem_ptr_mem_type().pointer();
        return val.cast(fptr_type);
    }

    /// Converts formatting result
    fmt_result_ref convert(const value & val, const fmt_result_ref & res) const override {
        return make_fmt_result<ot_proxy_fmt_result>(res, val.wqual_type_name());
    }
};


}
