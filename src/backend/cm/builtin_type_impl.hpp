
/// \file builtin_type_impl.hpp
/// Contains definition of the builtin_type_impl class.

#pragma once

#include "type_impl.hpp"
#include "dbgfmt/backend/builtin_type_impl.hpp"
#include <cm/cm.hpp>


namespace dbgfmt::backend::cm {


/// Builtin type implementation over CM
class builtin_type_impl: public cm_type_impl<::cm::builtin_type>,
                         virtual public dbgfmt::backend::builtin_type_impl {
public:
    /// Constructs builtin type implementation with specified reference to type context
    builtin_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::builtin_type> & ct):
        cm_type_impl<::cm::builtin_type>(tctx, ct) {}

    /// Returns kind of builtin type
    kind_t kind() const override {
        switch (cm_type().type()->kind()) {
        case ::cm::builtin_type::kind_t::void_:
            return dbgfmt::backend::builtin_type_impl::kind_t::void_;
        case ::cm::builtin_type::kind_t::bool_:
            return dbgfmt::backend::builtin_type_impl::kind_t::bool_;
        case ::cm::builtin_type::kind_t::char_:
            return dbgfmt::backend::builtin_type_impl::kind_t::char_;
        case ::cm::builtin_type::kind_t::short_:
            return dbgfmt::backend::builtin_type_impl::kind_t::short_;
        case ::cm::builtin_type::kind_t::int_:
            return dbgfmt::backend::builtin_type_impl::kind_t::int_;
        case ::cm::builtin_type::kind_t::long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::long_;
        case ::cm::builtin_type::kind_t::long_long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::long_long_;
        case ::cm::builtin_type::kind_t::unsigned_char_:
            return dbgfmt::backend::builtin_type_impl::kind_t::unsigned_char_;
        case ::cm::builtin_type::kind_t::unsigned_short_:
            return dbgfmt::backend::builtin_type_impl::kind_t::unsigned_short_;
        case ::cm::builtin_type::kind_t::unsigned_int_:
            return dbgfmt::backend::builtin_type_impl::kind_t::unsigned_int_;
        case ::cm::builtin_type::kind_t::unsigned_long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::unsigned_long_;
        case ::cm::builtin_type::kind_t::unsigned_long_long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::unsigned_long_long_;
        case ::cm::builtin_type::kind_t::float_:
            return dbgfmt::backend::builtin_type_impl::kind_t::float_;
        case ::cm::builtin_type::kind_t::double_:
            return dbgfmt::backend::builtin_type_impl::kind_t::double_;
        case ::cm::builtin_type::kind_t::long_double_:
            return dbgfmt::backend::builtin_type_impl::kind_t::long_double_;

        case ::cm::builtin_type::kind_t::wchar_t_:
            return dbgfmt::backend::builtin_type_impl::kind_t::wchar_t_;
        case ::cm::builtin_type::kind_t::char16_t_:
            return dbgfmt::backend::builtin_type_impl::kind_t::char16_t_;
        case ::cm::builtin_type::kind_t::char32_t_:
            return dbgfmt::backend::builtin_type_impl::kind_t::char32_t_;

        case ::cm::builtin_type::kind_t::nullptr_t_:
            return dbgfmt::backend::builtin_type_impl::kind_t::nullptr_t_;

        case ::cm::builtin_type::kind_t::complex_char_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_char_;
        case ::cm::builtin_type::kind_t::complex_short_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_short_;
        case ::cm::builtin_type::kind_t::complex_int_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_int_;
        case ::cm::builtin_type::kind_t::complex_long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_long_;
        case ::cm::builtin_type::kind_t::complex_long_long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_long_long_;
        case ::cm::builtin_type::kind_t::complex_unsigned_char_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_unsigned_char_;
        case ::cm::builtin_type::kind_t::complex_unsigned_short_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_unsigned_short_;
        case ::cm::builtin_type::kind_t::complex_unsigned_int_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_unsigned_int_;
        case ::cm::builtin_type::kind_t::complex_unsigned_long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_unsigned_long_;
        case ::cm::builtin_type::kind_t::complex_unsigned_long_long_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_unsigned_long_long_;
        case ::cm::builtin_type::kind_t::complex_float_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_float_;
        case ::cm::builtin_type::kind_t::complex_double_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_double_;
        case ::cm::builtin_type::kind_t::complex_long_double_:
            return dbgfmt::backend::builtin_type_impl::kind_t::complex_long_double_;

        default:
            assert(false && "Unknown CM builtin type kind");
            return dbgfmt::backend::builtin_type_impl::kind_t::void_;
        }
    }
};


}


