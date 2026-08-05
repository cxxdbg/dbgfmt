
/// \file std_bind_result_formatter.hpp
/// Contains definition of the std_bind_result_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


/// Base class for fomatters of result of std::bind. Contains common logic.
class std_bind_result_formatter: public formatter {
public:
    /// Formats value. Returns formatting result.
    fmt_result_ref format(const value & val) override;

private:
    /// Writes functor name to output stream
    void format_functor(std::wostream & str,
                        source_position & val_pos,
                        const value & val) const;

    /// Returns value containing bind target
    virtual value bind_target(const value & val) const = 0;

    /// Returns value containing bound arguments
    virtual value bound_arguments(const value & val) const = 0;
};


}
