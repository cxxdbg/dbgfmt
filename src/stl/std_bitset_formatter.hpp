
/// \file std_bitset_formatter.hpp
/// Contains definition of the std_bitset_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// Base class for formatters for std::bitset. Contains common formatting logic.
class std_bitset_formatter: public formatter {
public:
    /// Constructs bitset formatter for formatting bitset with specified
    /// name of data field in bitset record
    std_bitset_formatter(const std::string & d_f_name):
        data_fld_name_{d_f_name} {}

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;

private:
    /// Returns type name visible to user. Default implementation returns value type name
    virtual std::wstring disp_type_name(const value & val) {
        return val.wqual_type_name();
    }

    std::string data_fld_name_;         ///< Name of data field in bitset record
};


}
