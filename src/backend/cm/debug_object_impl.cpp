
/// \file debug_object_impl.cpp
/// Contains implementation of the debug_object_impl class.

#include "debug_object_impl.hpp"
#include <cm/source_location.hpp>
#include <cm/template_instantiation.hpp>
#include <cm/variable.hpp>
#include "type_context_impl.hpp"


namespace dbgfmt::backend::cm {


dbgfmt::backend::source_position debug_object_impl::def_pos() const {
    ::cm::source_location loc;

    if (auto ent = dynamic_cast<::cm::context_entity*>(ent_)) {
        loc = ent->loc();
    } else if (auto inst = dynamic_cast<::cm::template_instantiation*>(ent_)) {
        loc = inst->templ()->loc();
    } else {
        assert(false && "unknown type of value object");
    }

    if (loc) {
        return {loc.file()->path().string(), loc.line()};
    } else {
        return {};
    }
}


std::string variable_impl::name() const {
    std::ostringstream str;
    ::cm::cxx::print_entity(str, var_);
    return str.str();
}


type_impl * variable_impl::type() const {
    return ctx().get_or_create_type(var_->type());
}


std::string function_impl::name() const {
    std::ostringstream str;
    ::cm::cxx::print_function_name(str, func_);
    return str.str();
}


type_impl * function_impl::type() const {
    return ctx().get_or_create_type(ctx().cm().get_or_create_func_type(func_));
}


const record_type_impl * function_impl::record_type() const {
    auto dctx = func_->ctx();
    auto rec = dynamic_cast<::cm::record_type*>(dctx);
    if (!rec) {
        return nullptr;
    }

    auto type = ctx().get_or_create_type(rec);
    assert(type && "get_or_create_type returned null");
    auto rtype = dynamic_cast<record_type_impl*>(type);
    assert(rtype && "get_or_create_type returned not a record");
    return rtype;
}


}
