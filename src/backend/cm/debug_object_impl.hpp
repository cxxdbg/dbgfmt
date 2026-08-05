
/// \file debug_object_impl.hpp
/// Contains definition of the debug_object_impl class.

#pragma once

#include "type_impl.hpp"
#include "record_type_impl.hpp"
#include <cm/entity.hpp>
#include "dbgfmt/backend/debug_object_impl.hpp"


namespace dbgfmt::backend::cm {


/// FMT debug object implementation on top of CM
class debug_object_impl: virtual public dbgfmt::backend::debug_object_impl {
public:
    /// Constructs debug object implementation with specified reference to type context
    /// implementation pointer to CM object debug info, and pointer to CM object
    debug_object_impl(type_context_impl & ctx, ::cm::object_debug_info * dobj, ::cm::entity * ent):
        ctx_{ctx}, dbg_{dobj}, ent_{ent} {}

    /// Virtual destructor
    virtual ~debug_object_impl() = default;

    /// Returns address of object
    uint64_t addr() const override {
        return dbg_->addr();
    }

    /// Returns position of definition of object
    dbgfmt::backend::source_position def_pos() const override;

    /// Always returns nullptr
    const dbgfmt::backend::function_debug_object_impl * as_function() const override {
        return nullptr;
    }

protected:
    /// Returns reference to type context implementation
    type_context_impl & ctx() const { return ctx_; }

private:
    type_context_impl & ctx_;           ///< Reference to type context
    ::cm::object_debug_info * dbg_;       ///< Pointer to CM debug info for this object
    ::cm::entity * ent_;                  ///< Pointer to CM entity for this object
};


/// Debug object implementation for variable
class variable_impl: virtual public debug_object_impl {
public:
    /// Constructs debug object implementation for variable
    variable_impl(type_context_impl & ctx, ::cm::object_debug_info * dobj, ::cm::variable * var):
        cm::debug_object_impl{ctx, dobj, var}, var_{var} {}

    /// Returns variable name
    std::string name() const override;

    /// Returns variable type
    type_impl * type() const override;

private:
    ::cm::variable * var_;        ///< Pointer to code model variable
};


/// Debug object implementation for function
class function_impl: public debug_object_impl, virtual public dbgfmt::backend::function_debug_object_impl {
public:
    /// Constructs debug object implementation for function
    function_impl(type_context_impl & ctx, ::cm::object_debug_info * dobj, ::cm::function * func):
        cm::debug_object_impl{ctx, dobj, func}, func_{func} {}

    /// Returns function name
    std::string name() const override;

    /// Returns function type
    type_impl * type() const override;

    /// Returns this
    const function_impl * as_function() const override { return this; }

    /// Returns type of record for member function
    const record_type_impl * record_type() const override;

private:
    ::cm::function * func_;           ///< Pointer to code model variable
};


}


