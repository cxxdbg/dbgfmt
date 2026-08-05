
/// \file vector_formatter.hpp
/// Contains definition of the vector_formatter class.

#pragma once

#include "container_formatter.hpp"


namespace dbgfmt {


/// Formatter for vector (SSE/AVX) values
class vector_formatter: public container_formatter {
public:
    /// Should never be called, always returns false
    bool accepts(const value & v) const override {
        return v.is_vector();
    }

    /// Returns container size
    std::size_t size(const value & val) const override {
        return val.vector_size();
    }

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override {
        return val.vector_at(index);
    }
};


}
