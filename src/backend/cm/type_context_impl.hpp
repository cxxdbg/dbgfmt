
/// \file type_context_impl.hpp
/// Contains definition of the type_context_impl class.

#pragma once

#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/type_context_impl.hpp"


namespace dbgfmt::backend::cm {


/// Formatters type context implementation over code model
class type_context_impl: virtual public dbgfmt::backend::type_context_impl {
public:
    /// Constructs type context with specified referene to code model
    type_context_impl(::cm::code_model & cmcm, ::cm::debug_info & dbg):
        cm_ {cmcm}, dbg_{dbg} {}

    /// Returns pointer to implementation of value type
    /// with specified type name. Returns null pointer if type
    /// does not exist.
    type_impl * find_type(const std::string & nm) override;

    /// Returns builtin type of specified kind
    type_impl * bt_type(dbgfmt::backend::builtin_type_impl::kind_t kind) override;

    /// Gets or creates fmt type implementation for specified CM type
    type_impl * get_or_create_type(const ::cm::qual_type & ct);

    /// Returns builtin type of specified kind
    type_impl * bt_type(::cm::builtin_type::kind_t kind) {
        return get_or_create_type(cm_.bt_type(kind));
    }

    /// Returns reference to underlying code model
    auto & cm() { return cm_; }

    /// Returns reference to debug info
    auto & dbg() const { return dbg_; }

    // builtin type getters
    auto bt_void()                  { return get_or_create_type(cm_.bt_void()); }
    auto bt_bool()                  { return get_or_create_type(cm_.bt_bool()); }
    auto bt_char()                  { return get_or_create_type(cm_.bt_char()); }
    auto bt_short()                 { return get_or_create_type(cm_.bt_short()); }
    auto bt_int()                   { return get_or_create_type(cm_.bt_int()); }
    auto bt_long()                  { return get_or_create_type(cm_.bt_long()); }
    auto bt_long_long()             { return get_or_create_type(cm_.bt_long_long()); }
    auto bt_unsigned_char()         { return get_or_create_type(cm_.bt_unsigned_char()); }
    auto bt_unsigned_short()        { return get_or_create_type(cm_.bt_unsigned_short()); }
    auto bt_unsigned_int()          { return get_or_create_type(cm_.bt_unsigned_int()); }
    auto bt_unsigned_long()         { return get_or_create_type(cm_.bt_unsigned_long()); }
    auto bt_unsigned_long_long()    { return get_or_create_type(cm_.bt_unsigned_long_long()); }
    auto bt_float()                 { return get_or_create_type(cm_.bt_float()); }
    auto bt_double()                { return get_or_create_type(cm_.bt_double()); }
    auto bt_long_double()           { return get_or_create_type(cm_.bt_long_double()); }

    auto bt_wchar_t()               { return get_or_create_type(cm_.bt_wchar_t()); }
    auto bt_char16_t()              { return get_or_create_type(cm_.bt_char16_t()); }
    auto bt_char32_t()              { return get_or_create_type(cm_.bt_char32_t()); }

    auto bt_complex_char()          { return get_or_create_type(cm_.bt_complex_char()); }
    auto bt_complex_short()         { return get_or_create_type(cm_.bt_complex_short()); }
    auto bt_complex_int()           { return get_or_create_type(cm_.bt_complex_int()); }
    auto bt_complex_long()          { return get_or_create_type(cm_.bt_complex_long()); }
    auto bt_complex_long_long()     { return get_or_create_type(cm_.bt_complex_long_long()); }

    auto bt_complex_unsigned_char()          { return get_or_create_type(cm_.bt_complex_unsigned_char()); }
    auto bt_complex_unsigned_short()         { return get_or_create_type(cm_.bt_complex_unsigned_short()); }
    auto bt_complex_unsigned_int()           { return get_or_create_type(cm_.bt_complex_unsigned_int()); }
    auto bt_complex_unsigned_long()          { return get_or_create_type(cm_.bt_complex_unsigned_long()); }
    auto bt_complex_unsigned_long_long()     { return get_or_create_type(cm_.bt_complex_unsigned_long_long()); }

    auto bt_complex_float()         { return get_or_create_type(cm_.bt_complex_float()); }
    auto bt_complex_double()        { return get_or_create_type(cm_.bt_complex_double()); }
    auto bt_complex_long_double()   { return get_or_create_type(cm_.bt_complex_long_double()); }

    void set_template_params_enabled(bool enable) {
        template_params_enabled_ = enable;
    }

    bool template_params_enabled() const {
        return template_params_enabled_;
    }

private:
    /// Creates type corresponding to specified qual CM type
    std::shared_ptr<type_impl> create_type(const ::cm::qual_type & ct);

    /// Map from code model types and formatters type implementations
    std::map<::cm::const_qual_type, std::shared_ptr<type_impl>> types_;

    ::cm::code_model & cm_;               ///< Reference to code model
    ::cm::debug_info & dbg_;              ///< Reference to debug info for code model

    /// Global flag indicating if template parameter info is available
    /// for template instantiations.
    bool template_params_enabled_ = true;
};


}
