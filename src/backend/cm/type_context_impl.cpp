
/// \file type_context_impl.cpp
/// Contains implementation of the type_context_impl class.

#include "type_context_impl.hpp"
#include "array_type_impl.hpp"
#include "builtin_type_impl.hpp"
#include "enum_type_impl.hpp"
#include "function_type_impl.hpp"
#include "mem_ptr_type_impl.hpp"
#include "pointer_type_impl.hpp"
#include "record_type_impl.hpp"
#include "typedef_type_impl.hpp"
#include "vector_type_impl.hpp"
#include <cxxtn/tn.hpp>


namespace dbgfmt::backend::cm {


static ::cm::qual_type find_cm_type(::cm::code_model & m, const cxxtn::qual_type_name & tn);


/// Searches for decl context with specified parsed name
static ::cm::context * find_cm_decl_ctx(::cm::code_model & m, const cxxtn::scope_name * scnm) {
    auto sctn = dynamic_cast<const cxxtn::scope_type_name*>(scnm);
    if (!sctn) {
        // function scopes are not supported
        return nullptr;
    }

    // looking for parent decl context

    const cxxtn::scope_name * dctx_name = nullptr;
    if (auto btn = dynamic_cast<const cxxtn::basic_type_name*>(sctn)) {
        dctx_name = btn->scope();
    } else if (auto ttn = dynamic_cast<const cxxtn::template_type_name*>(sctn)) {
        dctx_name = ttn->template_name().scope();
    } else {
        assert(false && "unknown type name type");
    }

    ::cm::context * dctx = nullptr;
    if (dctx_name) {
        dctx = find_cm_decl_ctx(m, dctx_name);
        if (!dctx) {
            return nullptr;
        }
    } else {
        dctx = &m;
    }

    if (auto btn = dynamic_cast<const cxxtn::basic_type_name*>(sctn)) {
        // trying find nested namespace first
        if (auto ns = dynamic_cast<::cm::namespace_*>(dctx)) {
            auto nested_ns = ns->find_namespace(btn->identifier());
            if (nested_ns) {
                return nested_ns;
            }
        }

        // trying find nested record
        return dctx->find_named_record(btn->identifier());

    } else if (auto ttn = dynamic_cast<const cxxtn::template_type_name*>(sctn)) {
        // looking for template
        auto templ = dctx->find_template_record(ttn->template_name().identifier());
        if (!templ) {
            return nullptr;
        }

        // checking size of template parameters
        if (std::ranges::distance(templ->template_params()) != ttn->params().size()) {
            return nullptr;
        }

        // looking for template parameters

        std::vector<::cm::qual_type> params;
        params.reserve(ttn->params().size());

        for (auto && par_n : ttn->params()) {
            auto par_t = find_cm_type(m, par_n);
            if (!par_t) {
                return nullptr;
            }

            params.push_back(par_t);
        }

        // looking for template instantiation
        return templ->find_instantiation(params);

    } else {
        assert(false && "unknown scope name type");
        return nullptr;
    }
}


/// Converts builtin type name to builtin type from cm
static ::cm::builtin_type * find_cm_builtin_type(::cm::code_model & m,
                                               const cxxtn::builtin_type_name * btn) {
    switch (btn->kind()) {
    case cxxtn::builtin_type_name::kind_t::void_:
        return m.bt_void();
    case cxxtn::builtin_type_name::kind_t::char_:
        return m.bt_char();
    case cxxtn::builtin_type_name::kind_t::short_:
        return m.bt_short();
    case cxxtn::builtin_type_name::kind_t::int_:
        return m.bt_int();
    case cxxtn::builtin_type_name::kind_t::long_:
        return m.bt_long();
    case cxxtn::builtin_type_name::kind_t::long_long_:
        return m.bt_long_long();
    case cxxtn::builtin_type_name::kind_t::unsigned_char_:
        return m.bt_unsigned_char();
    case cxxtn::builtin_type_name::kind_t::unsigned_short_:
        return m.bt_unsigned_short();
    case cxxtn::builtin_type_name::kind_t::unsigned_int_:
        return m.bt_unsigned_int();
    case cxxtn::builtin_type_name::kind_t::unsigned_long_:
        return m.bt_unsigned_long();
    case cxxtn::builtin_type_name::kind_t::unsigned_long_long_:
        return m.bt_unsigned_long_long();
    case cxxtn::builtin_type_name::kind_t::float_:
        return m.bt_float();
    case cxxtn::builtin_type_name::kind_t::double_:
        return m.bt_double();
    case cxxtn::builtin_type_name::kind_t::long_double_:
        return m.bt_long_double();
    case cxxtn::builtin_type_name::kind_t::wchar_t_:
        return m.bt_wchar_t();
    default:
        assert(false && "unknown builtin type name kind");
        return nullptr;
    }
}


/// Searches for type with specified parsed type name in code model
static ::cm::qual_type find_cm_type(::cm::code_model & m, const cxxtn::qual_type_name & tn) {
    ::cm::type_t * type = nullptr;

    if (auto btn = dynamic_cast<const cxxtn::builtin_type_name*>(tn.type())) {
        type = find_cm_builtin_type(m, btn);
    } else if (auto scn = dynamic_cast<const cxxtn::scope_name*>(tn.type())) {
        // trying find scope with specified name and convert it to type
        type = dynamic_cast<::cm::type_t*>(find_cm_decl_ctx(m, scn));
    } else if (auto ftn = dynamic_cast<const cxxtn::function_type_name*>(tn.type())) {
        // looking for return type
        auto ret_type = find_cm_type(m, ftn->ret_type());
        if (!ret_type) {
            return {};
        }

        // looking for parametes
        std::vector<::cm::qual_type> params;
        for (auto && par_name : ftn->params()) {
            auto par = find_cm_type(m, par_name);
            if (!par) {
                return {};
            }

            params.push_back(par);
        }

        // creating function type
        type = m.get_or_create_func_type_r(ret_type, params);
    } else if (auto ptn = dynamic_cast<const cxxtn::pointer_type_name*>(tn.type())) {
        auto base = find_cm_type(m, ptn->base());
        if (!base) {
            return {};
        }

        type = m.get_or_create_ptr_type(base);
    } else if (auto rtn = dynamic_cast<const cxxtn::reference_type_name*>(tn.type())) {
        auto base = find_cm_type(m, rtn->base());
        if (!base) {
            return {};
        }

        type = m.get_or_create_lvalue_ref_type(base);
    }

    return ::cm::qual_type{type, tn.is_const(), tn.is_volatile()};
}


type_impl * type_context_impl::find_type(const std::string & nm) {
    // parsing type name
    std::istringstream istr{nm};
    auto name = cxxtn::parse_type_name(istr);
    if (!name.type()) {
        return nullptr;
    }

    // trying find type with parsed name
    auto type = find_cm_type(cm(), name);
    if (!type) {
        return nullptr;
    }

    return get_or_create_type(type);
}


type_impl * type_context_impl::bt_type(builtin_type_impl::kind_t kind) {
    switch (kind) {
    case builtin_type_impl::kind_t::void_:
        return bt_void();
    case builtin_type_impl::kind_t::bool_:
        return bt_bool();
    case builtin_type_impl::kind_t::char_:
        return bt_char();
    case builtin_type_impl::kind_t::short_:
        return bt_short();
    case builtin_type_impl::kind_t::int_:
        return bt_int();
    case builtin_type_impl::kind_t::long_:
        return bt_long();
    case builtin_type_impl::kind_t::long_long_:
        return bt_long_long();
    case builtin_type_impl::kind_t::unsigned_char_:
        return bt_unsigned_char();
    case builtin_type_impl::kind_t::unsigned_short_:
        return bt_unsigned_short();
    case builtin_type_impl::kind_t::unsigned_int_:
        return bt_unsigned_int();
    case builtin_type_impl::kind_t::unsigned_long_:
        return bt_unsigned_long();
    case builtin_type_impl::kind_t::unsigned_long_long_:
        return bt_unsigned_long_long();
    case builtin_type_impl::kind_t::float_:
        return bt_float();
    case builtin_type_impl::kind_t::double_:
        return bt_long_double();
    case builtin_type_impl::kind_t::long_double_:
        return bt_long_double();
    case builtin_type_impl::kind_t::wchar_t_:
        return bt_wchar_t();
    case builtin_type_impl::kind_t::char16_t_:
        return bt_char16_t();
    case builtin_type_impl::kind_t::char32_t_:
        return bt_char32_t();
    case builtin_type_impl::kind_t::nullptr_t_:
        throw dbgfmt::backend::format_error{"can't create nullptr_t builtin type"};
    case builtin_type_impl::kind_t::complex_char_:
        return bt_complex_char();
    case builtin_type_impl::kind_t::complex_short_:
        return bt_complex_short();
    case builtin_type_impl::kind_t::complex_int_:
        return bt_complex_int();
    case builtin_type_impl::kind_t::complex_long_:
        return bt_complex_long();
    case builtin_type_impl::kind_t::complex_long_long_:
        return bt_complex_long_long();
    case builtin_type_impl::kind_t::complex_unsigned_char_:
        return bt_complex_unsigned_char();
    case builtin_type_impl::kind_t::complex_unsigned_short_:
        return bt_complex_unsigned_short();
    case builtin_type_impl::kind_t::complex_unsigned_int_:
        return bt_complex_unsigned_int();
    case builtin_type_impl::kind_t::complex_unsigned_long_:
        return bt_complex_unsigned_long();
    case builtin_type_impl::kind_t::complex_unsigned_long_long_:
        return bt_complex_unsigned_long_long();
    case builtin_type_impl::kind_t::complex_float_:
        return bt_complex_float();
    case builtin_type_impl::kind_t::complex_double_:
        return bt_complex_double();
    case builtin_type_impl::kind_t::complex_long_double_:
        return bt_complex_long_double();
    default:
        throw dbgfmt::backend::format_error{"Unknown builtin type kind"};
    }
}


type_impl * type_context_impl::get_or_create_type(const ::cm::qual_type & ct) {
    // looking for existing type in the map
    auto & res = types_[ct];
    if (res) {
        return res.get();
    }

    // creating new type
    res = create_type(ct);
    assert(res && "created type should not be null");

    return res.get();
}


std::shared_ptr<type_impl> type_context_impl::create_type(const ::cm::qual_type & ct) {
    if (auto bct = ct.cast<::cm::builtin_type>()) {
        return std::make_shared<builtin_type_impl>(*this, bct);
    } else if (auto tdt = ct.cast<::cm::typedef_type>()) {
        return std::make_shared<typedef_type_impl>(*this, tdt);
    } else if (auto et = ct.cast<::cm::enum_type>()) {
        return std::make_shared<enum_type_impl>(*this, et);
    } else if (auto at = ct.cast<::cm::array_type>()) {
        return std::make_shared<array_type_impl>(*this, at);
    } else if (auto vt = ct.cast<::cm::vector_type>()) {
        return std::make_shared<vector_type_impl>(*this, vt);
    } else if (auto ptr = ct.cast<::cm::ptr_or_ref_type>()) {
        return std::make_shared<pointer_type_impl>(*this, ptr);
    } else if (auto mptr = ct.cast<::cm::mem_ptr_type>()) {
        return std::make_shared<mem_ptr_type_impl>(*this, mptr);
    } else if (auto rt = ct.cast<::cm::record_type>()) {
        return std::make_shared<record_type_impl>(*this, rt);
    } else if (auto ft = ct.cast<::cm::function_type>()) {
        return std::make_shared<function_type_impl>(*this, ft);
    } else {
        assert(false && "NYI");
    }

    return {};
}


}
