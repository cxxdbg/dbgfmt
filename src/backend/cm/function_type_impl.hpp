
/// \file function_type_impl.hpp
/// Contains definition of the function_type_impl class.

#pragma once

#include "type_context_impl.hpp"
#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/function_type_impl.hpp"


namespace dbgfmt::backend::cm {


class function_type_impl: public cm_type_impl<::cm::function_type>,
                          virtual public dbgfmt::backend::function_type_impl {
public:
    /// Constructs function type implementation with specified CM qual type
    function_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::function_type> & ct):
        cm_type_impl<::cm::function_type>(tctx, ct) {}

    /// Returns pointer to return type
    const cm::type_impl * ret_type() const override {
        return fmtcm_ctx().get_or_create_type(cm_type()->ret_type());
    }

    /// Returns number of parameters
    size_t params_size() const override {
        return std::ranges::size(cm_type()->params());
    }

    /// Returns type of parameter at specified index
    const cm::type_impl * param_at(size_t idx) const override {
        return fmtcm_ctx().get_or_create_type(cm_type()->params()[idx]);
    }
};


}


