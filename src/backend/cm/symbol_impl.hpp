
/// \file symbol_impl.hpp
/// Contains definition of the symbol_impl class.

#pragma once

#include "dbgfmt/backend/symbol_impl.hpp"
#include <cm/cm.hpp>


namespace dbgfmt::backend::cm {


class symbol_impl: public dbgfmt::backend::symbol_impl {
public:
    /// Constructs symbol implementation with specified pointer to
    /// CM debug symbol
    symbol_impl(const ::cm::debug_symbol * sym): sym_{sym} {}

    /// Virtual destructor
    virtual ~symbol_impl() = default;

    /// Returns address of symbol
    uint64_t addr() const override {
        return sym_->addr();
    }

    /// Returns name of symbol
    std::string name() const override {
        return sym_->name();
    }

    /// Returns size of symbol
    uint64_t size() const override {
        return sym_->size();
    }

private:
    const ::cm::debug_symbol * sym_;      ///< Pointer to CM debug symbol
};


}


