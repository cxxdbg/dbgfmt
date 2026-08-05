
/// \file temp_value_impl.hpp
/// Contains definition of the temp_value_impl class.

#pragma once

#include "value_impl_common.hpp"
#include "source_position.hpp"
#include "data/vector_storage.hpp"


namespace dbgfmt {

namespace backend { class value_context_impl; }


/// Implementation of value that has its own temporary data storage
class temp_value_impl: public value_impl_common {
public:
    /// Constructs temporary value of specified type and allocates new temporary
    /// storage of size of specified type
    temp_value_impl(backend::value_context_impl & c, const backend::type_impl * t, data::storage & mem, data::endianness e):
        temp_value_impl(c, t, std::make_shared<data::vector_storage>(t->size()), 0, mem, e) {}

    /// Constructs temporary value of specified type with existing temporary storage
    temp_value_impl(backend::value_context_impl & c,
                    const backend::type_impl * t,
                    const std::shared_ptr<data::storage> & st,
                    uint64_t offs,
                    data::storage & mem,
                    data::endianness e):
        value_impl_common{c},
        type_{t}, data_{st}, offset_{offs}, memory_{mem}, end_{e} {}

    /// Returns pointer to implementation of value type
    const backend::type_impl * raw_type() const override { return type_; }

    /// Returns size of value
    std::size_t size() const override {
        return type_->size();
    }

    /// Returns reference to data storage for value
    data::storage & storage() const override {
        return *data_;
    }

    /// Returns offset in data storage for value
    uint64_t offset() const override {
        return offset_;
    }

    /// Returns data endianness for value
    dbgfmt::data::endianness endianness() const override {
        return end_;
    }

    /// Creates value with specified type, data storage, endianness, and offset
    backend::value_impl_sp create_value(const backend::type_impl * type,
                               uint64_t offs,
                               const source_position & def_pos) const override {
        return std::make_shared<temp_value_impl>(*ctx(),
                                                 type,
                                                 data_,
                                                 offs,
                                                 memory_,
                                                 end_);
    }

    /// Returns reference to memory storage
    data::storage & memory() const override {
        return memory_;
    }

    /// Returns data accessor for value
    data::accessor data() const {
        return data::accessor{storage(), endianness(), offset()};
    }

private:
    const backend::type_impl * type_;            ///< Pointer to value type implementation

    /// Shared pointer to temporary storage for value
    std::shared_ptr<data::storage> data_;

    uint64_t offset_;                   ///< Offset in temporary storage
    data::storage & memory_;            ///< Reference to memory storage
    data::endianness end_;              ///< Storage endianness
};


}


