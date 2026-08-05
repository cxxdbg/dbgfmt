
/// \file value_context_impl.hpp
/// Contains definition of the value_context_impl class.

#pragma once

#include "debug_object_impl.hpp"
#include <cm/entity.hpp>
#include "symbol_impl.hpp"
#include "type_context_impl.hpp"
#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/value_context_impl.hpp"
#include <map>


namespace dbgfmt::backend::cm {


/// Value context implementation on top of code model and debug info
class value_context_impl: public dbgfmt::backend::value_context_impl {
public:
    /// Constructs context implementation with specified reference to type context
    value_context_impl(type_context_impl & tctx): tctx_{tctx} {}

    /// Tries detect dynamic value type at specified address. Tries find type for address
    /// added with add_dynamic_type function.
    const type_impl * dynamic_type_at_addr(uint64_t & addr, const dbgfmt::backend::type_impl * static_type) override;

    /// Returns function name containing at specified address, and offset in the function
    dbgfmt::backend::debug_object_impl * obj_at_addr(uint64_t addr) override {
        // looking for object in CM debug info
        auto cm_obj = tctx().dbg().entity_at_addr(addr);
        if (!cm_obj) {
            return nullptr;
        }

        // looking for existing debug_object_impl in the map of debug objects
        auto & res = debug_objects_[cm_obj];
        if (res) {
            return res.get();
        }

        // creating new debug_object_impl
        auto cm_dbg = tctx().dbg().object(cm_obj);
        if (auto var = dynamic_cast<::cm::variable*>(cm_obj)) {
            res = std::make_unique<variable_impl>(tctx(), cm_dbg, var);
        } else if (auto func = dynamic_cast<::cm::function*>(cm_obj)) {
            res = std::make_unique<function_impl>(tctx(), cm_dbg, func);
        } else {
            assert(false && "don't know how to make debug object for cm object");
        }

        return res.get();
    }

    /// Returns symbol located at specified address
    symbol_impl * sym_at_addr(uint64_t addr) override {
        // looking for symbol in the CM debug info
        auto sym = tctx().dbg().sym_at_addr(addr);
        if (!sym) {
            return nullptr;
        }

        // looking for existing symbol_impl the map of symbols
        auto & res = symbols_[sym];
        if (res) {
            return res.get();
        }

        // creating new symbol_impl object
        res = std::make_unique<symbol_impl>(sym);
        return res.get();
    }

    /// Returns reference to type context
    type_context_impl & tctx() { return tctx_; }

    /// Adds dynamic type mapping for address
    void add_dynamic_type(uint64_t addr, const type_impl * dyn_type, uint64_t dyn_addr) {
        dynamic_types_.emplace(addr, dynamic_type_info{dyn_type, dyn_addr});
    }

private:
    type_context_impl & tctx_;      ///< Reference to type context

    /// Map of debug objects
    std::unordered_map<const ::cm::entity*, std::unique_ptr<debug_object_impl>> debug_objects_;

    /// Map of symbols in executable
    std::unordered_map<const ::cm::debug_symbol*, std::unique_ptr<symbol_impl>> symbols_;

    struct dynamic_type_info {
        const type_impl * type;
        uint64_t addr;
    };

    /// Map of dynamic value types for addresses
    std::map<uint64_t, dynamic_type_info> dynamic_types_;
};


}
