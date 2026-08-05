
/// \file typedef_type_impl.hpp
/// Contains definition of the typedef_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


/// Represents abstract typedef type implementation
class typedef_type_impl: virtual public type_impl {
public:
    /// Returns base type for typedefed type
    virtual const type_impl * base() const = 0;
};


}


