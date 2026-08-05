
/// \file stdcpp_queue_formatter.hpp
/// Contains definition of the libstdc++ queue formatter.

#pragma once

#include "stdcpp_deque_formatter.hpp"


namespace dbgfmt {


/// libstdc++ queue formatter
class stdcpp_queue_formatter: public stdcpp_deque_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override;

private:
    /// Returns container size
    std::size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;
};


}


