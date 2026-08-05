
/// \file stdcpp_deque_formatter.hpp
/// Contains definition of stdc++ deque formatter classes.

#pragma once

#include "../../container_formatter.hpp"
#include "../../container_iterator_formatter.hpp"


namespace dbgfmt {


/// libstdc++ deque formatter
class stdcpp_deque_formatter: public container_formatter {
public:
    /// Returns true if formatter accepts specified value.
    virtual bool accepts(const value & v) const;

protected:
    /// Returns container size
    virtual std::size_t size(const value & val) const;

    /// Returns value object for container element with specified index
    virtual value element(const value & val, std::size_t index) const;
};


/// libstdc++ deque iterator formatter
class stdcpp_deque_iterator_formatter: public container_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    virtual bool accepts(const value & v) const;

private:
    /// Returns pointer to value pointed by iterator
    virtual value get_value_pointer(const value & it) const;
};


}


