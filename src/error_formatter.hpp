
/// \file error_formatter.hpp
/// Contains definition of the error_formatter class.

#pragma once

#include "simple_formatter.hpp"
#include <sstream>


namespace dbgfmt {


/// Formatter for error values
class error_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override { return v.is_error(); }

private:
    /// Writes value to output stream
    void write(const value & val, std::wostream & str) const override {
        str << L"<" << val.err_msg() << L">";
    }
};


}


