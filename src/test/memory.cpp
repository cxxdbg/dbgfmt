
/// \file memory.cpp
/// Contains implementation of the memory class.

#include "memory.hpp"
#include <cassert>
#include <sstream>


namespace dbgfmt::test {


storage * memory::alloc_block(uint64_t offset, size_t sz) {
    auto block = std::make_unique<vector_storage>(sz);
    auto block_ptr = block.get();
    add_block(offset, std::move(block));

    return block_ptr;
}


void memory::read(void * data, size_t sz, uint64_t offset) {
    iterate_blocks(reinterpret_cast<uint8_t*>(data), sz, offset,
    [](auto block, auto d, auto sz, auto offs) {
        block->read(d, sz, offs);
    });
}


void memory::write(const void * data, size_t sz, uint64_t offset) {
    iterate_blocks(reinterpret_cast<const uint8_t*>(data), sz, offset,
    [](auto block, auto d, auto sz, auto offs) {
        block->write(d, sz, offs);
    });
}

void memory::add_block(uint64_t offset, std::unique_ptr<vector_storage> block) {
    // adding block into map
    auto res = blocks_.emplace(offset, std::move(block));
    assert(res.second && "data block with specified offset already exists");

    auto curr = res.first;

    // checking for overlap with previous block
    if (curr != blocks_.begin()) {
        auto prev = curr;
        --prev;

        assert(prev->first + prev->second->data().size() <= curr->first &&
               "memory block overlapped with previous block");
    }

    // checking for overlap with next block
    auto next = curr;
    ++next;
    if (next != blocks_.end()) {
        assert(curr->first + curr->second->data().size() <= next->first &&
               "memory block overlapped with next block");
    }
}


template <typename DataType, typename Op>
void memory::iterate_blocks(DataType data, size_t sz, uint64_t offset, const Op & op) {
    // looking for first data block containing specified offset
    auto block_it = blocks_.upper_bound(offset);
    if (block_it == blocks_.begin()) {
        std::ostringstream msg;
        msg << "can't find memory block containing data block ("
            << offset << ", " << sz << ")";
        throw data::error{msg.str()};
    }

    --block_it;

    // checking that block contains starting offset
    if (block_it->first + static_cast<uint64_t>(block_it->second->size()) <= offset) {
        std::ostringstream msg;
        msg << "can't find memory block containing data block ("
            << offset << ", " << sz << ")";
        throw data::error{msg.str()};
    }


    // calculating offset and size in the first block

    auto read_offset = offset - block_it->first;

    size_t nread = 0;

    while (true) {
        assert(nread < sz && "invalid number of bytes read");

        // reading data from current data block

        assert(read_offset < block_it->second->size() && "invalid read offset");
        auto read_size = sz - nread;
        if (read_size + read_offset > block_it->second->size()) {
            read_size = block_it->second->size() - read_offset;
        }

        // applying data operation
        op(block_it->second.get(), data, read_size, read_offset);

        data += read_size;
        nread += read_size;


        // checking for end of data read
        if (nread == sz) {
            break;
        }


        // read offset is always zero on all iterations except first
        read_offset = 0;


        // checking that current and next data blocks are placed
        // continuously in memory

        auto next_it = block_it;
        ++next_it;

        if (block_it->first + block_it->second->size() != next_it->first) {
            std::ostringstream msg;
            msg << "can't find continuous memory block of size " << sz
                << " containing at offset " << offset;
            throw data::error{msg.str()};
        }

        block_it = next_it;
    }
}


}
