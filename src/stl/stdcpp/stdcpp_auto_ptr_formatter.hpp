
/// \file stdcpp_auto_ptr_formatter.hpp
/// Contains definition of the stdcpp_auto_ptr_formatter class.

#pragma once

#include "../../smart_ptr_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ auto_ptr class.
class stdcpp_auto_ptr_formatter: public smart_ptr_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override;

private:
    /// Returns value for real pointer
    value get_pointer(const value & v) const override;
};


}


