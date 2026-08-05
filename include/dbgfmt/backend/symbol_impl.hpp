
/// \file symbol_impl.hpp
/// Contains definition of the symbol_impl class.

#pragma once

#include <cstdint>
#include <string>


namespace dbgfmt::backend {


/// Represents abstract implementation of symbol in executable being
/// debugged
class symbol_impl {
public:
    /// Returns address of symbol
    virtual uint64_t addr() const = 0;

    /// Returns name of symbol
    virtual std::string name() const = 0;

    /// Returns size of symbol
    virtual uint64_t size() const = 0;
};


}


