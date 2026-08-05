
/// \file typedef_type_impl.hpp
/// Contains definition of the typedef_type_impl class.

#pragma once

#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/typedef_type_impl.hpp"


namespace dbgfmt::backend::cm {


/// FMT typedef type implementation on top of CM typedef type
class typedef_type_impl: public cm_type_impl<::cm::typedef_type>,
                         virtual public dbgfmt::backend::typedef_type_impl {
public:
    /// Constructs typedef type impl with specified CM typedef type
    typedef_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::typedef_type> & cmt):
        cm_type_impl<::cm::typedef_type>(tctx, cmt) {}

    /// Returns base type for typedefed type
    const cm::type_impl * base() const override {
        return fmtcm_ctx().get_or_create_type(cm_type().type()->base());
    }
};


}


