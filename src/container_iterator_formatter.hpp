
/// \file container_iterator_formatter.hpp
/// Contains definition of the container_iterator_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"


namespace dbgfmt {


/// \file container_iterator_formatter
/// Formatter for container iterators
class container_iterator_formatter: public formatter {
public:
    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;

private:
    /// Returns pointer to value pointed by iterator
    virtual value get_value_pointer(const value & it) const = 0;
};


}


