
/// \file libcxx_regex_formatter.hpp
/// Contains definitions of formatter classes for libc++ regex types.

#pragma once

#include "dbgfmt/formatter.hpp"
#include "../std_string_formatter.hpp"


namespace dbgfmt {


/// Formatter for libc++ std::sub_match type
class libcxx_sub_match_formatter: public std_string_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

protected:
    /// Returns false
    bool has_capacity() const override { return false; }

    /// Adds [begin] and [end] childs into result
    fmt_result_ref transform_result(const value & val, const fmt_result_ref & res) override;

    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


/// Formatter for libc++ std::match_results type
class libcxx_match_results_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

protected:
    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


}
