
/// \file enum_type_impl.hpp
/// Contains definition of the enum_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


/// Abstract implementation of the enum type
class enum_type_impl: virtual public type_impl {
public:
    /// Returns number of enum items
    virtual std::size_t items_size() const = 0;

    /// Returns name of enum item with specified index
    virtual std::string item_name(std::size_t index) const = 0;

    /// Returns value of enum item with specified index as signed integer
    virtual std::int64_t item_value(std::size_t index) const = 0;
};


}


