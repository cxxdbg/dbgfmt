
/// \file pointer_type_impl.hpp
/// Contains definition of the pointer_type_impl class.

#pragma once

#include "type_context_impl.hpp"
#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/pointer_type_impl.hpp"


namespace dbgfmt::backend::cm {


class code_model;


/// Pointer type implementation over CM types
class pointer_type_impl: public cm_type_impl<::cm::ptr_or_ref_type>,
                         virtual public dbgfmt::backend::pointer_type_impl {
public:
    /// Constructs pointer type with specified pointer to base type and "is reference" flag
    pointer_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::ptr_or_ref_type> & ct):
        cm::cm_type_impl<::cm::ptr_or_ref_type>(tctx, ct) {}

    /// Returns pointer to value context impl
    dbgfmt::backend::type_context_impl * ctx() const override {
        return base()->ctx();
    }

    /// Returns true if pointer type is reference
    bool is_reference() const override {
        return cm_type()->is_ref();
    }

    /// Returns pointer to base type implementation
    const cm::type_impl * base() const override {
        return fmtcm_ctx().get_or_create_type(cm_type()->base());
    }

    /// Returns true if pointer can be dereferenced
    bool can_dereference() const override;
};


}


