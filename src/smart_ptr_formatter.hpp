
/// \file smart_ptr_formatter.hpp
/// Contains definition of the smart_ptr_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Represents abstract formatter for smart pointer
class smart_ptr_formatter: public formatter {
public:
    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;

private:
    /// Returns value for real pointer
    virtual value get_pointer(const value & v) const = 0;

    /// Returns number of extra childs which should be displayed by formatter.
    /// Default implementation returns 0.
    virtual std::size_t extra_childs_size() const;

    /// Returns name of additional child with specified index. Default implementation
    /// calls assert(false) and returns empty string
    virtual std::wstring extra_child_name(size_t idx) const;

    /// Returns additional child with specified index. Default implementation
    /// calls assert(false) and returns null pointer.
    virtual fmt_result_ref extra_child(const value & v, std::size_t index) const;

    /// Returns type name displayed for user. Default implementation returns
    /// qualified type name of value
    virtual std::wstring disp_type_name(const value & v) const;
};


}


