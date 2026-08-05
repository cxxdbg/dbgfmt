
/// \file auto_memory.cpp
/// Contains implementation of the auto_memory class.

#include "auto_memory.hpp"
#include <cassert>


namespace dbgfmt::test {


uint64_t auto_memory::alloc(size_t sz) {
    assert(sz != 0 && "Can't allocate block of size 0");
    auto offset = curr_offset_;
    alloc_block(offset, sz);
    curr_offset_ += sz + space_size_;
    return offset;
}


uint64_t auto_memory::alloc_error(size_t sz) {
    assert(sz != 0 && "Can't allocate block of size 0");
    auto offset = curr_offset_;
    curr_offset_ += sz + space_size_;
    return offset;
}


}
