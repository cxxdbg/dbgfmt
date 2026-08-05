
/// \file mem_ptr_type_impl.hpp
/// Contains definition of the mem_ptr_type_impl class.

#pragma once

#include "record_type_impl.hpp"
#include "type_context_impl.hpp"
#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/mem_ptr_type_impl.hpp"


namespace dbgfmt::backend::cm {


/// Member pointer type implementation over CM types
class mem_ptr_type_impl: public cm_type_impl<::cm::mem_ptr_type>,
                         virtual public dbgfmt::backend::mem_ptr_type_impl {
public:
    /// Constructs pointer type with specified qual type
    mem_ptr_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::mem_ptr_type> & ct):
        cm::cm_type_impl<::cm::mem_ptr_type>(tctx, ct) {}

    /// Returns pointer to type of member
    const dbgfmt::backend::type_impl * mem_type() const override {
        return fmtcm_ctx().get_or_create_type(cm_type()->mem_type());
    }

    /// Returns pointer to type of object
    const cm::record_type_impl * obj_type() const override {
        auto res = fmtcm_ctx().get_or_create_type(cm_type()->obj_type());
        auto rres = dynamic_cast<cm::record_type_impl*>(res);
        assert(rres && "get_or_create_type should return record");
        return rres;
    }
};


}


