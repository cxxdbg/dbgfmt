

/// \file old_mem_fun_formatter.hpp
/// Contains definition of the old_mem_fun_formatter class

#pragma once

#include "value_conv_formatter.hpp"


namespace dbgfmt {


/// Formatter for member function in old representation
class old_mem_fun_formatter: public value_conv_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & val) const override {
        return val.is_record() &&
           val.type_name() == "(anonymous struct)" &&
           val.has_field("__pfn") &&
           val.has_field("__delta");
    }

private:
    /// Converts value for formatting
    value convert(const value & val) const override {
        return val["__pfn"];
    }
};


}
