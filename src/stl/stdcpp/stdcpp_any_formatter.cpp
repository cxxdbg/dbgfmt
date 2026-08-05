
/// \file stdcpp_any_formatter.cpp
/// Contains implementation of the stdcpp_any_formatter class.

#include "stdcpp_any_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "../../proxy_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include <cxxtn/tn.hpp>


namespace dbgfmt {


bool stdcpp_any_formatter::accepts(const value & v) const {
    return v.is_record() && v.type_name() == "std::any";
}


fmt_result_ref stdcpp_any_formatter::format(const value & val) {
    // checking for empty function
    if (val["_M_manager"].as_uint64() == 0) {
        auto res = std::make_unique<fixed_fmt_result>(L"[value = <empty>]",
                                                      val.wqual_type_name(),
                                                      source_position{},
                                                      source_position{});
        res->add_child(L"[value]", [val] {
            return make_fmt_result<fixed_fmt_result>(L"<empty>",
                                                     std::wstring{},
                                                     source_position{},
                                                     source_position{});
        });

        return make_ref(std::move(res));
    }

    // getting name of manager function
    auto man_name = val["_M_manager"].function_name();
    if (man_name.empty()) {
        throw format_error{"Can't get name of std::any manager"};
    }


    // parsing function manager name
    std::istringstream str{man_name};
    cxxtn::qual_type_name man_type;
    cxxtn::scope_type_name_sp man_decl_name;
    cxxtn::parse_declaration(str, man_type, man_decl_name);
    if (!man_type.is_valid()) {
        std::ostringstream msg;
        msg << "Can't parse std::any manager name: '" << man_name << "'";
        throw format_error{msg.str()};
    }

    // checking that function manager name is a basic type name
    auto man_basic_name = dynamic_cast<const cxxtn::basic_type_name*>(man_decl_name.get());
    if (!man_basic_name) {
        std::ostringstream msg;
        msg << "std::any manager name ' " << man_name << "' is not a basic type name";
        throw format_error{msg.str()};
    }

    // getting scope of function manager name

    if (!man_basic_name->has_scope()) {
        std::ostringstream msg;
        msg << "std::any manager name ' " << man_name << "' does not have scope";
        throw format_error{msg.str()};
    }

    auto man_basic_name_scope = man_basic_name->scope();
    assert(man_basic_name_scope && "should have scope is has_scope() returns true");

    // checking that function manager name scope is a template
    auto man_templ_name = dynamic_cast<const cxxtn::template_type_name*>(man_basic_name_scope);
    if (!man_templ_name || man_templ_name->params().size() == 0) {
        std::ostringstream msg;
        msg << "std::any manager name '" << man_name
            << "' scope is not a template containing at least one parameter";
        throw format_error{msg.str()};
    }

    // getting stored type
    auto stored_type_name = man_templ_name->params()[0];
    std::ostringstream stored_type_name_str;
    stored_type_name.write(stored_type_name_str, cxxtn::type_name::write_style::gcc);
    auto stored_type = ctx().find_type(stored_type_name_str.str());

    value stored_value;

    auto mname = man_templ_name->template_name().identifier();
    if (mname == "_Manager_internal") {
        // internal 
        stored_value = val["_M_storage"]["_M_buffer"]["__data"].cast(stored_type);
    } else if (mname == "_Manager_external") {
        // external data
        stored_value = *val["_M_storage"]["_M_ptr"].cast(stored_type.pointer());
    } else {
        std::ostringstream msg;
        msg << "unknown std::any manager name: '" << mname << "'";
        throw format_error{msg.str()};
    }

    auto formatted_value = ctx().format(stored_value);

    // if formatted functor contains value position then use it as value pos
    // for entire result
    source_position val_def_pos;
    if (formatted_value.val_pos()) {
        val_def_pos = formatted_value.val_pos();
    }

    // formatting result value
    std::wostringstream val_str;
    val_str << "[value = " << formatted_value.val() << "]";

    // creating fixed fmt result
    auto res = std::make_unique<fixed_fmt_result>(val_str.str(),
                                                  val.wqual_type_name(),
                                                  val_def_pos,
                                                  val.type().def_pos());

    res->add_child(L"[value]", [formatted_value]() { return formatted_value; });

    return make_ref(std::move(res));
}


}
