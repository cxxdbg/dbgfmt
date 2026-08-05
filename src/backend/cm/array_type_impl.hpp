
/// \file array_type_impl.hpp
/// Contains definition of the array_type_impl class.

#pragma once

#include "type_context_impl.hpp"
#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/array_type_impl.hpp"


namespace dbgfmt::backend::cm {


/// Array type implemntation over CM
class array_type_impl: public cm_type_impl<::cm::array_type>,
                       virtual public dbgfmt::backend::array_type_impl {
public:
    /// Consturcts array type with specified base type
    array_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::array_type> & ct):
        cm_type_impl<::cm::array_type>(tctx, ct) {}

    /// Returns pointer to implementation of base type
    const cm::type_impl * base() const override {
        return fmtcm_ctx().get_or_create_type(cm_type()->base());
    }

    /// Returns size of array
    size_t array_size() const override {
        return cm_type()->size();
    }
};


}


