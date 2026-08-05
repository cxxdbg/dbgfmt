
/// \file vector_storage.hpp
/// Contains definition of the vector_storage class.

#pragma once

#include "storage.hpp"
#include "error.hpp"
#include <algorithm>
#include <vector>


namespace dbgfmt::data {


/// Data storage with vector backend
class vector_storage: public storage {
public:
    /// Constructs vector storage with specified size of initial data
    /// initialized to zero
    vector_storage(size_t sz = 0):
        data_(sz) {}

    /// Constructs storage with specified data bytes
    vector_storage(const std::initializer_list<unsigned char> & data) {
        data_.reserve(data.size());
        std::copy(data.begin(), data.end(), std::back_inserter(data_));
    }

    /// Reads bytes from storage at specified offset
    void read(void * data, size_t sz, uint64_t offset = 0) override {
        // size of size_t may be less than size of uint64_t
        auto end_offset = static_cast<uint64_t>(sz) + offset;
        if (end_offset > static_cast<uint64_t>(data_.size())) {
            throw error{"Can't read after the end of vector storage"};
        }

        auto uchar_data = reinterpret_cast<unsigned char*>(data);
        auto sz_offs = static_cast<size_t>(offset);
        std::copy(data_.begin() + sz_offs, data_.begin() + sz_offs + sz, uchar_data);
    }

    /// Writes bytes to storage at specified offset
    void write(const void * data, size_t sz, uint64_t offset) override {
        // size of size_t may be less than size of uint64_t
        auto end_offset = static_cast<uint64_t>(sz) + offset;
        if (end_offset > static_cast<uint64_t>(data_.size())) {
            throw error{"Can't write after the end of vector storage"};
        }

        auto uchar_data = reinterpret_cast<const unsigned char*>(data);
        auto sz_offs = static_cast<size_t>(offset);
        std::copy(uchar_data, uchar_data + sz, data_.begin() + sz_offs);
    }

    /// Append bytes to data storage
    void append(const std::initializer_list<unsigned char> & bytes) {
        data_.reserve(data_.size() + bytes.size());
        std::copy(bytes.begin(), bytes.end(), std::back_inserter(data_));
    }

    /// Returns reference to storage vector
    auto & data() { return data_; }

    /// Returns reference to storage vector
    auto & data() const { return data_; }

    /// Returns size of data
    auto size() { return data().size();}

private:
    std::vector<unsigned char> data_;       ///< Vector of data
};


}
