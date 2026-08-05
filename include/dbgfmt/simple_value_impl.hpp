
/// \file simple_value_impl.hpp
/// Contains definition of the simple_value_impl class.

#pragma once

#include "value_impl_common.hpp"
#include "source_position.hpp"
#include "data/accessor.hpp"
#include "format_error.hpp"
#include "backend/value_impl.hpp"
#include <sstream>


namespace dbgfmt {

namespace backend { class value_context_impl; }
class simple_value_impl;
using simple_value_impl_sp = std::shared_ptr<simple_value_impl>;


/// Simple value implementation that holds pointer to data storage and value type
class simple_value_impl: public value_impl_common {
public:
    /// Constructs simple value implementation with specified type, reference to
    /// data storage, endianned, and offset in data storage
    explicit simple_value_impl(backend::value_context_impl & c,
                               const backend::type_impl * type,
                               data::storage & d,
                               data::endianness e,
                               uint64_t offs,
                               const source_position & dpos = {}):
        value_impl_common{c},
        type_{type}, data_{d}, endianness_{e}, offset_{offs}, def_pos_{dpos} {}

    /// Constructs simple value implementation with specified type and data accessor
    explicit simple_value_impl(backend::value_context_impl & c,
                               const backend::type_impl * type,
                               const data::accessor & d,
                               const source_position & dpos = {}):
        simple_value_impl(c, type, d.stor(), d.endian(), d.offset(), dpos) {}

    /// Returns pointer to implementation of value type
    const backend::type_impl * raw_type() const override { return type_; }

    /// Returns size of value
    std::size_t size() const override { return type_->size(); }

    /// Returns pointer to data storage for value
    data::storage & storage() const override { return data_; }

    /// Returns offset in data storage for value
    uint64_t offset() const override { return offset_; }

    /// Returns data endianness for value
    dbgfmt::data::endianness endianness() const override { return endianness_; }

    /// Creates value that points to same data storage as this value
    backend::value_impl_sp create_value(const backend::type_impl * type,
                               uint64_t offs,
                               const source_position & def_pos) const override {
        return std::make_shared<simple_value_impl>(*ctx(),
                                                   type,
                                                   storage(),
                                                   endianness(),
                                                   offs,
                                                   def_pos);
    }

    /// Returns reference to memory storage
    data::storage & memory() const override { return data_; }

private:
    const backend::type_impl * type_;            ///< Pointer to value type implementation
    data::storage & data_;              ///< Reference to data storage for value
    data::endianness endianness_;       ///< Value endiannes
    uint64_t offset_;                   ///< Value offset in data storage
    source_position def_pos_;           ///< Position of value definition
};


}


