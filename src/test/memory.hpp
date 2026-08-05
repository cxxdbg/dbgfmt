
/// \file memory.hpp
/// Contains definition of the memory class

#pragma once

#include "dbgfmt/data/storage.hpp"
#include "dbgfmt/data/vector_storage.hpp"
#include <map>


namespace dbgfmt::test {


using storage = data::storage;
using vector_storage = data::vector_storage;


/// Implementation of memory simulator that contains multiple
/// blocks of allocated memory blocks
class memory: public data::storage {
public:
    /// Constructs empty memory storage with no blocks
    memory() = default;

    /// Allocates new data block of specified size at specified memory offset.
    /// Returns storage object that represents it.
    storage * alloc_block(uint64_t offset, size_t sz);

    /// Reads bytes from storage at specified offset.
    /// Throws error if data can't be read.
    void read(void * data, size_t sz, uint64_t offset) override;

    /// Writes bytes to storage at specified offset
    void write(const void * data, size_t sz, uint64_t offset) override;

    /// Adds block into list of allocated blocks
    void add_block(uint64_t offset, std::unique_ptr<vector_storage> block);

private:
    /// Iterates over coninuously allocated memory blocks and
    /// applies read/write operation for each block
    template <typename DataType, typename Op>
    void iterate_blocks(DataType data, size_t sz, uint64_t offset, const Op & op);


    /// Map of data blocks
    std::map<uint64_t, std::unique_ptr<vector_storage>> blocks_;
};


}


