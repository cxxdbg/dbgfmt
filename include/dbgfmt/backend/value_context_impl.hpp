
/// \file value_context_impl.hpp
/// Contains definition of the value_context_impl class.

#pragma once

#include "debug_object_impl.hpp"
#include "symbol_impl.hpp"
#include "type_impl.hpp"
#include "value_impl.hpp"
#include "../data/storage.hpp"
#include "../data/endianness.hpp"


namespace dbgfmt::backend {


/// Interface for value context implementation. Contains
/// functions for globally searching of types and values in target
class value_context_impl {
public:
    /// Returns referen to storage for target memory
    virtual data::storage & memory() = 0;

    /// Returns endianness of target
    virtual data::endianness endianness() const = 0;

    /// Makes temporary value of specified type in host memory
    virtual std::shared_ptr<value_impl> make_temp_val(const type_impl * type) = 0;

    /// Tries detect dynamic value type and address at specified address.
    /// Returns dynamic type and fills dyn_addr with dynamic start address for that type.
    virtual const type_impl * dynamic_type_at_addr(uint64_t & addr, const type_impl * static_type) = 0;

    /// Returns object located at specified address or nullptr if object not found
    virtual debug_object_impl * obj_at_addr(uint64_t addr) = 0;

    /// Returns symbol name located at specified address
    virtual symbol_impl * sym_at_addr(uint64_t addr) = 0;
};


}


