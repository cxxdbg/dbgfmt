
#pragma once

#include "type_context_impl.hpp"
#include "type_impl.hpp"
#include "dbgfmt/backend/format_error.hpp"
#include "dbgfmt/backend/record_type_impl.hpp"
#include <cm/cm.hpp>
#include <cm/cxx/print.hpp>
#include <cassert>


namespace dbgfmt::backend::cm {


/// Returns name of a function entity, including template arguments for
/// template function instantiations (which aren't named_function
/// themselves, unlike plain and template functions)
inline std::string cm_func_name(const ::cm::function * func) {
    if (auto tfi = dynamic_cast<const ::cm::template_function_instantiation*>(func)) {
        return tfi->name_with_templ_params();
    } else if (auto nf = dynamic_cast<const ::cm::named_function*>(func)) {
        return nf->name();
    }

    assert(false && "function without a name");
    return {};
}


/// Implementation of record type on top of code model library
class record_type_impl: public cm_type_impl<::cm::record_type>,
                        virtual public dbgfmt::backend::record_type_impl {
public:
    /// Constructs formatters record type with specified reference to type context and qual CM type
    record_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::record_type> & ct):
    cm_type_impl<::cm::record_type>(tctx, ct) {
        for (auto && var : this->cm_type()->fields()) {
            fields_.push_back(var);
        }
    }

    /// Returns type name taking into account anonymous records
    std::string name() const override {
        if (!cm_type().cast<::cm::named_record_type>() &&
            !cm_type().cast<::cm::template_record_instantiation_type>()) {
            // special case for anonymous record. Returns same value as lldb
            auto get_record_kind = [](::cm::record_kind knd) -> std::string {
                switch (knd) {
                case ::cm::record_kind::class_:
                    return "class";
                case ::cm::record_kind::struct_:
                    return "struct";
                case ::cm::record_kind::union_:
                    return "union";
                default:
                    assert(false && "unknown record kind");
                    return {};
                }
            };

            return std::string{"(unnamed "} + get_record_kind(cm_type()->kind()) + ")";
        }


        return type_impl::name();
    }

    /// Returns name of template class without template parameters
    std::string template_name() const override {
        if (auto inst = cm_type().cast<::cm::template_record_instantiation_type>()) {
            std::ostringstream str;
            ::cm::cxx::print_entity(str, inst->templ());
            return str.str();
        }

        return name();
    }

    /// Returns number of template parameters
    std::size_t template_param_names_size() const override {
        auto inst = cm_type().type()->cast<::cm::template_record_instantiation_type>();
        if (!inst) {
            return 0;
        }

        return std::ranges::size(inst->args());
    }

    /// Returns template parameter with specified index
    std::string template_param_name(std::size_t index) const override {
        if (index >= template_param_names_size()) {
            std::ostringstream msg;
            msg << "can't find template parameter with index " << index << " for type '" << qual_name() << "'";
            throw dbgfmt::backend::format_error{msg.str()};
        }

        auto inst = cm_type().type()->cast<::cm::template_record_instantiation_type>();
        assert(inst && "can't convert type to template record instance");

        auto arg = inst->args()[index];
        if (auto t_arg = dynamic_cast<const ::cm::type_template_argument*>(arg)) {
            std::ostringstream str;
            ::cm::cxx::print_type(str, t_arg->type());
            return str.str();
        } else if (auto v_arg = dynamic_cast<const ::cm::value_template_argument*>(arg)) {
            return v_arg->val().str();
        } else {
            assert(false && "unknown template instantiation argument type");
            return {};
        }
    }


    /// Returns number of template parameters
    size_t template_params_size() const override {
        // if enable flag is false in type context then disable access to
        // template params info. This is needed to test correct work of some
        // formatter for old compilers where template parameter info is not
        // available
        if (!fmtcm_ctx().template_params_enabled()) {
            return 0;
        }

        return template_param_names_size();
    }


    /// Returns true if template parameter is a type
    bool template_param_is_type(size_t index) const override {
        if (index >= template_params_size()) {
            std::ostringstream msg;
            msg << "can't find template parameter with index " << index << " for type '" << qual_name() << "'";
            throw dbgfmt::backend::format_error{msg.str()};
        }

        auto inst = cm_type().type()->cast<::cm::template_record_instantiation_type>();
        assert(inst && "can't convert type to template record instance");

        auto arg = inst->args()[index];
        return dynamic_cast<const ::cm::type_template_argument*>(arg) != nullptr;
    }


    /// Returns template parameter type
    const dbgfmt::backend::type_impl * template_param_type(size_t index) const override {
        if (!template_param_is_type(index)) {
            std::ostringstream msg;
            msg << "template parameter at index " << index << " for type '" << qual_name() << "' is not a type";
            throw dbgfmt::backend::format_error{msg.str()};
        }

        auto inst = cm_type().type()->cast<::cm::template_record_instantiation_type>();
        assert(inst && "can't convert type to template record instance");

        auto arg = inst->args()[index];
        auto t_arg = dynamic_cast<::cm::type_template_argument*>(arg);
        assert(t_arg && "template argument is not a type");
        return fmtcm_ctx().get_or_create_type(t_arg->type());
    }


    /// Returns true if record is dynamic type (has vtable)
    bool is_dynamic() const override {
        return false;
    }


    /// Returns number of direct base classes
    std::size_t bases_size() const override {
        return std::ranges::size(cm_type()->bases());
    }

    /// Returns type of direct base class at specified index
    const record_type_impl * base_type_at(std::size_t i) const override {
        check_base_index(i);

        auto cm_base = cm_type().type()->bases()[i];
        auto base = fmtcm_ctx().get_or_create_type(cm_base);
        auto r_base = dynamic_cast<const record_type_impl*>(base);
        assert(r_base && "get_or_create_type returned invalid type");
        return r_base;
    }

    /// Returns offset of base class value at specified index
    uint64_t base_offset_at(size_t i, uint64_t obj_addr) const override {
        check_base_index(i);

        auto dbg_info_untyped = fmtcm_ctx().dbg().type(cm_type());
        auto dbg_info = dynamic_cast<const ::cm::record_type_debug_info*>(dbg_info_untyped);
        assert(dbg_info && "debug info for record type not found");

        return dbg_info->bases_offsets()[i];
    }

    /// Returns number of fields
    std::size_t fields_size() const override {
        return fields_.size();
    }

    /// Returns name of field with specified index
    std::string field_name_at(std::size_t i) const override {
        check_field_index(i);
        return fields_[i]->name();
    }

    /// Returns source position of definition of record field at
    /// specified index. Returns invalid source position if not avaliable.
    dbgfmt::backend::source_position field_def_pos_at(size_t i) const override {
        check_field_index(i);
        auto & loc = fields_[i]->loc();

        if (loc) {
            return {loc.file()->path().string(), loc.line()};
        } else {
            return {};
        }
    }

    /// Returns value of field with specified index
    dbgfmt::backend::type_impl * field_type_at(std::size_t i) const override {
        check_field_index(i);
        auto cmt = fields_[i]->type();
        return fmtcm_ctx().get_or_create_type(cmt);
    }

    /// Returns offset of field in bits with specified index
    uint64_t field_offset_at(size_t i) const override {
        check_field_index(i);
        auto dbg_info_nt = fmtcm_ctx().dbg().type(cm_type());
        auto dbg_info = dynamic_cast<const ::cm::record_type_debug_info*>(dbg_info_nt);
        if (!dbg_info) {
            throw dbgfmt::backend::format_error{"can't find debug info for record"};
        }

        assert(i < dbg_info->vars_offsets().size() && "invalid debug info");
        return dbg_info->vars_offsets()[i];
    }

    /// Returns size in bits of field with specified index
    uint64_t field_size_at(size_t i) const override {
        check_field_index(i);
        auto var = fields_[i];
        if (var->bit_size() == 0) {
            return fmtcm_ctx().dbg().type_size(var->type()) * 8;
        } else {
            return var->bit_size();
        }
    }

    /// Finds field in record type taking into account base classes if check_bases is true.
    const dbgfmt::backend::type_impl * find_field(const std::string & nm,
                                      uint64_t * offset,
                                      uint64_t * size,
                                      dbgfmt::backend::source_position * def_pos,
                                      bool check_bases = true) const override {

        // pass pointer to debug info to find_field only if we need get offset or size
        auto dinfo = (offset == nullptr && size == nullptr) ? nullptr : &fmtcm_ctx().dbg();

        ::cm::field * var = nullptr;
        uint64_t offs = 0;
        std::tie(var, offs) = ::cm::find_field(dinfo, cm_type().type(), nm, check_bases);

        if (!var) {
            return nullptr;
        }

        if (offset) {
            *offset = offs;
        }

        if (size) {
            if (var->bit_size() != 0) {
                *size = var->bit_size();
            } else {
                *size = fmtcm_ctx().dbg().type_size(var->type()) * 8;
            }
        }

        return fmtcm_ctx().get_or_create_type(var->type());
    }

    /// Rerurns type of member function with specified name, or 0 if function
    /// with such name not found
    dbgfmt::backend::type_impl * mem_fun_type(const std::string & nm) const override {
        // searching for member function with specified name
        auto entities = cm_type()->entities();
        auto it = std::ranges::find_if(entities, [&nm](auto && ent) {
            if (auto func = dynamic_cast<::cm::function*>(ent)) {
                return cm_func_name(func) == nm;
            } else {
                return false;
            }
        });

        if (it == std::ranges::end(entities)) {
            // function not found
            return nullptr;
        }

        auto func = dynamic_cast<::cm::function*>(*it);
        assert(func && "invalid iterator value");
        return fmtcm_ctx().get_or_create_type(fmtcm_ctx().cm().get_or_create_func_type(func));
    }

    /// Returns address of member function with specified name, or 0 if
    /// address of function not found
    uint64_t mem_fun_addr(const std::string & nm) const override {
        // searching for member function with specified name
        auto entities = cm_type()->entities();
        auto it = std::ranges::find_if(entities, [&nm](auto && ent) {
            if (auto func = dynamic_cast<::cm::function*>(ent)) {
                return cm_func_name(func) == nm;
            } else {
                return false;
            }
        });

        if (it == std::ranges::end(entities)) {
            // function not found
            return 0;
        }

        // getting address of function from debug info
        auto dbg_obj = fmtcm_ctx().dbg().object(*it);
        if (!dbg_obj) {
            return 0;
        }

        return dbg_obj->addr();
    }

    /// Returns vector of names of member functions of record
    std::vector<std::string> mem_funcs() const override {
        std::vector<std::string> res;

        for (auto && ent : cm_type()->entities()) {
            auto func = dynamic_cast<const ::cm::function*>(ent);
            if (!func) {
                continue;
            }

            // skipping templates
            if (dynamic_cast<const ::cm::template_function*>(func)) {
                continue;
            }

            res.push_back(cm_func_name(func));
        }

        return res;
    }

private:
    /// Checks base class index
    void check_base_index(size_t idx) const {
        if (idx >= bases_size()) {
            std::ostringstream msg;
            msg << "base class index " << idx << " is out of range";
            throw dbgfmt::backend::format_error{msg.str()};
        }
    }

    /// Checks field index
    void check_field_index(size_t idx) const {
        if (idx >= fields_size()) {
            std::ostringstream msg;
            msg << "field index " << idx << " is out of range";
            throw dbgfmt::backend::format_error{msg.str()};
        }
    }

    /// Vector of record fields
    std::vector<::cm::field*> fields_;
};


}


