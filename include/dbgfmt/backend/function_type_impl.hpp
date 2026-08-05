
/// \file function_type_impl.hpp
/// Contains definition of the function_type_impl class.

#pragma once

#include "type_impl.hpp"


namespace dbgfmt::backend {


/// Interface for implementation of function type
class function_type_impl: virtual public type_impl {
public:
    /// Returns pointer to return type
    virtual const type_impl * ret_type() const = 0;

    /// Returns number of parameters
    virtual size_t params_size() const = 0;

    /// Returns type of parameter at specified index
    virtual const type_impl * param_at(size_t idx) const = 0;
};


}


