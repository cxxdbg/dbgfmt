
/// \file stdcpp_function_formatter.cpp
/// Contains implementation of formatter for libstdc++ function.

#include "stdcpp_function_formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include "dbgfmt/context.hpp"
#include "../../proxy_fmt_result.hpp"
#include <cxxtn/tn.hpp>
#include "../../log.hpp"
#include <memory>
#include <iostream>


namespace dbgfmt {


bool stdcpp_function_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::function";
}


fmt_result_ref stdcpp_function_formatter::format(const value & val) {
    // checking for empty function
    if (val["_M_manager"].as_uint64() == 0) {
        auto res = std::make_unique<fixed_fmt_result>(L"[functor = <empty>]",
                                                      val.wqual_type_name(),
                                                      source_position{},
                                                      source_position{});
        res->add_child(L"[functor]", [val] {
            return make_fmt_result<fixed_fmt_result>(L"<empty>",
                                                     std::wstring{},
                                                     source_position{},
                                                     source_position{});
        });

        return make_ref(std::move(res));
    }

    fmt_result_ref functor_val;
    source_position functor_type_def_pos;

    // first trying use new API for accessing template parameters in debug info
    try {
        // getting manager function
        auto man_func = ctx().obj_at_addr(val["_M_manager"].as_uint64()).as_function();

        // getting manager type
        auto man_type = man_func.record_type();

        // getting functor type
        value_type functor_type;
        if (man_type.template_name() == "std::_Function_handler") {
            functor_type = man_type.template_param_type(1);
        } else {
            functor_type = man_type.template_param_type(0);
        }

        if (functor_type.is_mem_ptr() ||
            functor_type.is_pointer() && functor_type.ptr_base().is_function()) {
            // functor type is a pointer to function or pointer to member function

            // formatting pointer to function
            auto void_ptr_type = ctx().find_type("void").pointer();
            auto fptr = val["_M_functor"]["_M_pod_data"].cast(functor_type);
            functor_val = ctx().format(fptr);
        } else {
            // functor is a custom type or lambda
            
            // detecting if functor is inline (containing directly in pod data)
            // or outline (pod data contains pointer to functor).
            value functor;
            if (is_inline_functor(functor_type)) {
                // inline functor
                functor = val["_M_functor"]["_M_pod_data"].cast(functor_type);
            } else {
                // outline functor
                functor = *val["_M_functor"]["_M_pod_data"].cast(functor_type.pointer());
            }

            assert(functor.is_valid() && "functor should be valid here");

            // formatting functor value
            functor_val = ctx().format(functor);

            functor_type_def_pos = functor_type.def_pos();
        }
    }
    catch (format_error & err) {
        // using type name parsing below as backup path

        DBGFMT_LOG_ERROR << "error while formatting std::function using new API: " << err.what();

        // getting name of manager function
        auto man_func_name = val["_M_manager"].function_name();
        if (man_func_name.empty()) {
            throw format_error{"Can't get name of std::function manager"};
        }

        // parsing function manager name
        std::istringstream str{man_func_name};
        cxxtn::qual_type_name man_func_type;
        cxxtn::scope_type_name_sp man_decl_name;
        cxxtn::parse_declaration(str, man_func_type, man_decl_name);
        if (!man_func_type.is_valid()) {
            std::ostringstream msg;
            msg << "Can't parse std::function manager name: '" << man_func_name << "'";
            throw format_error{msg.str()};
        }

        // checking that function manager name is a basic type name
        auto man_basic_name = dynamic_cast<const cxxtn::basic_type_name*>(man_decl_name.get());
        if (!man_basic_name) {
            std::ostringstream msg;
            msg << "std::function manager name ' " << man_func_name << "' is not a basic type name";
            throw format_error{msg.str()};
        }

        // getting scope of function manager name

        if (!man_basic_name->has_scope()) {
            std::ostringstream msg;
            msg << "std::function manager name ' " << man_func_name << "' does not have scope";
            throw format_error{msg.str()};
        }

        auto man_basic_name_scope = man_basic_name->scope();
        assert(man_basic_name_scope && "should have scope is has_scope() returns true");

        // checking that function manager name scope is a template
        auto man_templ_name = dynamic_cast<const cxxtn::template_type_name*>(man_basic_name_scope);
        if (!man_templ_name || man_templ_name->params().size() == 0) {
            std::ostringstream msg;
            msg << "std::function manager name '" << man_func_name
                << "' scope is not a template containing at least one parameter";
            throw format_error{msg.str()};
        }

        cxxtn::qual_type_name functor_type_name;
        if (man_templ_name->template_name().identifier() == "_Function_handler") {
            // pointer to member function. Type of function contained in second parameter
            if (man_templ_name->params().size() < 2) {
                std::ostringstream msg;
                msg << "std::function manager name '" << man_func_name
                    << "' scope is not a template containing at least two parameters";
                throw format_error{msg.str()};
            }

            functor_type_name = man_templ_name->params()[1];

        } else {
            // other cases
            functor_type_name = man_templ_name->params()[0];
        }

        std::ostringstream functor_name_str;
        functor_type_name.write(functor_name_str, cxxtn::type_name::write_style::gcc);
        auto functor_name = functor_name_str.str();

        // checking if functor type is simple pointer to function
        auto ptr = dynamic_cast<const cxxtn::pointer_type_name*>(functor_type_name.type());
        auto mem_ptr = dynamic_cast<const cxxtn::mem_ptr_type_name*>(functor_type_name.type());

        if (mem_ptr ||
            (ptr && dynamic_cast<const cxxtn::function_type_name*>(ptr->base().type()))) {

            // formatting pointer to function as void*
            auto void_ptr_type = ctx().find_type("void").pointer();
            auto fptr = val["_M_functor"]["_M_pod_data"].cast(void_ptr_type);
            functor_val = ctx().format(fptr);

            std::wstring wfunctor_name{functor_name.begin(), functor_name.end()};
            functor_val = make_fmt_result<ot_proxy_fmt_result>(functor_val, wfunctor_name);
        } else {
            // looking for functor type
            auto functor_type = ctx().find_type(functor_name);

            // detecting if functor is inline (containing directly in pod data)
            // or outline (pod data contains pointer to functor).
            value functor;
            if (is_inline_functor(functor_type)) {
                // inline functor
                functor = val["_M_functor"]["_M_pod_data"].cast(functor_type);
            } else {
                // outline functor
                functor = *val["_M_functor"]["_M_pod_data"].cast(functor_type.pointer());
            }

            assert(functor.is_valid() && "functor should be valid here");

            // formatting functor value
            functor_val = ctx().format(functor);

            functor_type_def_pos = functor_type.def_pos();
        }
    }


    // if formatted functor contains value position then use it as value pos
    // for entire result
    source_position val_def_pos;
    if (functor_val.val_pos()) {
        val_def_pos = functor_val.val_pos();
    } else {
        val_def_pos = functor_type_def_pos;
    }

    // formatting result value
    std::wostringstream val_str;
    val_str << "[functor = " << functor_val.val() << "]";

    // creating fixed fmt result
    auto res = std::make_unique<fixed_fmt_result>(val_str.str(),
                                                  val.wqual_type_name(),
                                                  val_def_pos,
                                                  val.type().def_pos());

    res->add_child(L"[functor]", [functor_val]() { return functor_val; });

    return make_ref(std::move(res));
}


bool stdcpp_function_formatter::is_inline_functor(const value_type & functor_type) {
    // functors with size more than 2 x sizeof pointer is always outline
    if (functor_type.size() > 2 * ctx().address_size()) {
        return false;
    }

    // result of std::bind is always outline because it's not trivially copyaible
    static const std::string brtn = "std::_Bind<";
    if (functor_type.name().size() > brtn.size() &&
        functor_type.name().substr(0, brtn.size()) == brtn) {
        return false;
    }


    // TODO: can we check if user defined functor is trivially copyable?
    return true;
}


}
