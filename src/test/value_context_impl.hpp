
/// \file value_context_impl.hpp
/// Contains definition of the value_context_impl class.

#pragma once

#include "auto_memory.hpp"
#include "../backend/cm/fmtcm.hpp"
#include "dbgfmt/data/accessor.hpp"
#include "dbgfmt/data/endianness.hpp"


namespace dbgfmt::test {


/// Value context implementation for testing
class value_context_impl: public dbgfmt::backend::cm::type_context_impl, public dbgfmt::backend::cm::value_context_impl {
public:
    /// Constructs context with specified reference to code model and debug info
    value_context_impl(cm::code_model & c, cm::debug_info & d, data::endianness end):
        dbgfmt::backend::cm::type_context_impl{c, d},
        dbgfmt::backend::cm::value_context_impl{static_cast<dbgfmt::backend::cm::type_context_impl&>(*this)},
        end_{end} {}

    /// Returns reference to memory storage
    auto_memory & memory() override { return memory_; }

    /// Returns data endianness
    data::endianness endianness() const override { return end_; }

    /// Makes temporary value of specified type
    std::shared_ptr<backend::value_impl> make_temp_val(const backend::type_impl * type) override;

    /// Allocates and returns data for specified type
    data::accessor alloc_data(const cm::const_qual_type & type) {
        auto sz = dbg().type_size(type);
        auto offs = memory().alloc(sz);
        return {memory(), endianness(), offs};
    }

    /// Allocates and returns error data for specified type
    data::accessor alloc_error_data(const cm::const_qual_type & type) {
        auto sz = dbg().type_size(type);
        return {memory(), endianness(), memory().alloc_error(sz)};
    }

private:
    /// Data endianness
    data::endianness end_;

    /// Memory for value allocation
    auto_memory memory_;
};


}
