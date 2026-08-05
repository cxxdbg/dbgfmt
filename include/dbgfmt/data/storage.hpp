
/// \file storage.hpp
/// Contains definition of the storage class.

#pragma once

#include <cstddef>
#include <memory>
#include <vector>


namespace dbgfmt::data {


/// Represents abstract storage containing binary data
class storage {
public:
    /// Destroys object
    virtual ~storage() = default;

    /// Reads bytes from storage at specified offset.
    /// Throws error if data can't be read.
    virtual void read(void * data, size_t sz, uint64_t offset) = 0;

    /// Writes bytes to storage at specified offset
    virtual void write(const void * data, size_t sz, uint64_t offset) = 0;

    /// Writes bytes to storage from initializer list
    void write(uint64_t offset, const std::initializer_list<uint8_t> & vals) {
        std::vector<uint8_t> vvals(vals);
        write(&vvals[0], vvals.size(), offset);
    }

    /// Reads data from storage as vector of uint8_t
    std::vector<uint8_t> read_vector(uint64_t offset, size_t sz) {
        std::vector<uint8_t> res(sz);
        read(&res[0], sz, offset);
        return res;
    }

    /// Writes vector of uint8_t to data storage
    void write_vector(uint64_t offset, const std::vector<uint8_t> & v) {
        write(&v[0], v.size(), offset);
    }
};


using storage_sp = std::shared_ptr<storage>;


}
