
/// \file vector_type_impl.hpp
/// Contains definition of the vector_type_impl class.

#pragma once

#include "type_context_impl.hpp"
#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/vector_type_impl.hpp"


namespace dbgfmt::backend::cm {


/// Vector type implemntation over CM
class vector_type_impl: public cm_type_impl<::cm::vector_type>,
                       virtual public dbgfmt::backend::vector_type_impl {
public:
    /// Consturcts vector type with specified base type
    vector_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::vector_type> & ct):
        cm_type_impl<::cm::vector_type>(tctx, ct) {}

    /// Returns pointer to implementation of base type
    const cm::type_impl * base() const override {
        return fmtcm_ctx().get_or_create_type(cm_type()->base());
    }

    /// Returns size of vector
    size_t vector_size() const override {
        return cm_type()->size();
    }
};


}
