
/// \file value_type_impl_accessor.hpp
/// Contains definition of the value_type_impl_accessor class.

#pragma once

#include "dbgfmt/value_type.hpp"


namespace dbgfmt {


/// Helper class for accessing implementation of type
class value_type_impl_accessor {
public:
    /// Constructs accessor with specified reference to value type
    value_type_impl_accessor(const value_type & t): type_{t} {}

    /// Returns implementation of value type
    const backend::type_impl * get() const {
        return type_.impl();
    }

private:
    value_type type_;
};


}
