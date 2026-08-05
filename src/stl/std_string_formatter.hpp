
/// \file std_string_formatter.hpp
/// Contains definition of the std_string_formatter class.

#pragma once

#include "../container_formatter.hpp"


namespace dbgfmt {


/// Base class for string formatters. Contains commong logic
class std_string_formatter: public container_formatter {
protected:
    /// Always returns true
    bool has_capacity() const override;

    /// Formats value as string literal
    std::wstring format_value(const value & val) const override;

private:
    /// Formats string value to output stream
    template <typename Char>
    void format_string(std::wostream & str, const value & val) const;
};


}
