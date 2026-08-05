
/// \file auto_memory.hpp
/// Contains definition of the auto_memory class.

#pragma once

#include "memory.hpp"


namespace dbgfmt::test {


/// Implementation of memory simulator that can automatically allocate
/// memory blocks of specified size
class auto_memory: public memory {
public:
    /// Constructs empty memory storage with specified base address for
    /// automatically allocated blocks and space between blocks
    auto_memory(uint64_t base = 0x80000000, uint64_t sp = 100):
        curr_offset_{base}, space_size_{sp} {}

    /// Allocates new data block of specified size after the last allocated block.
    /// Returns offset of allocated block
    uint64_t alloc(size_t sz);

    /// Allocates new data block that can't be read. Attempt to read data
    /// from block always throws error. Return offset of allocated block.
    uint64_t alloc_error(size_t sz);

private:
    uint64_t curr_offset_;          ///< Current offset for new allocated blocks
    uint64_t space_size_;           ///< Size of space between allocated blocks
};


}


