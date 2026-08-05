
/// \file libcxx_deque_formatter.hpp
/// Contains definition of formatter for libc++ deque.

#pragma once

#include "../../container_formatter.hpp"
#include "../../container_iterator_formatter.hpp"


namespace dbgfmt {


/// libc++ deque formatter
class libcxx_deque_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

protected:
    /// Returns container size
    size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, size_t index) const override;

    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override;
};


/// libc++ deque iterator formatter
class libcxx_deque_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

private:
    /// Returns pointer to value pointed by iterator
    value get_value_pointer(const value & it) const override;
};


}

