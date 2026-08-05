
/// \file accessor.hpp
/// Contains definition of the accessor class.

#pragma once

#include "endianness.hpp"
#include "error.hpp"
#include "storage.hpp"
#include <algorithm>
#include <vector>


namespace dbgfmt::data {


/// Data accessor that is used for reading and writing data to storage
class accessor {
public:
    /// Constructs accessor with specified reference to data storage,
    /// endiannes, and offset
    accessor(storage & st, endianness e = endianness::little, uint64_t offs = 0) noexcept:
        storage_{st}, end_{e}, offset_{offs} {}

    /// Creates reader from another reader and offset. Throws error
    /// if total base offset in storage is negative
    accessor(const accessor & rdr, int64_t offs):
    storage_{rdr.storage_}, end_{rdr.end_}, offset_{rdr.offset_ + offs} {
        if (offs > rdr.offset_) {
            throw error{"can't read data from negative base offset"};
        }
    }

    /// Creates and returns new data accessor with specified additional
    /// offset relative to current base offset
    accessor operator+(int64_t offs) const {
        return accessor(*this, offs);
    }

    /// Destroys object
    virtual ~accessor() = default;

    /// Returns reference to storage
    auto & stor() const { return storage_; }

    /// Returns endianness of data accessor
    auto endian() const { return end_; }

    /// Sets endianness of data accessor
    void set_endian(endianness e) noexcept { end_ = e; }

    /// Returns offset in storage of data accessor
    auto offset() const { return offset_; }

    /// Reads unsigned integer of specified size
    uint64_t read_unsigned(size_t sz, uint64_t offset = 0) const;

    /// Reads signed integer of specified size
    int64_t read_signed(size_t sz, uint64_t offset = 0) const;

    /// Reads floating point value of specified size
    long double read_float(size_t sz, uint64_t offset = 0) const;

    /// Reads signed integer bits of specified size
    int64_t read_signed_bits(size_t bits_sz,
                             uint64_t byte_offset,
                             unsigned int bit_offset) const;

    /// Reads unsigned integer bits of specified size
    uint64_t read_unsigned_bits(size_t bits_sz,
                                uint64_t byte_offset,
                                unsigned int bit_offset) const;

    /// Writes unsigned integer of specified size
    void write_unsigned(uint64_t val, size_t sz, uint64_t offset = 0);

    /// Writes signed integer of specified size
    void write_signed(int64_t val, size_t sz, uint64_t offset = 0);

    /// Writes floating point value of specified size
    void write_float(long double val, size_t sz, uint64_t offset = 0);

    /// Writes unsigned integer of speicifed size
    void write(uint64_t val, size_t sz, uint64_t offset = 0) {
        write_unsigned(val, sz, offset);
    }

    /// Writes signed integer of specified size
    void write(int64_t val, size_t sz, uint64_t offset = 0) {
        write_signed(val, sz, offset);
    }

    /// Wrties floating point value of specified size
    void write(long double val, size_t sz, uint64_t offset = 0) {
        write_float(val, sz, offset);
    }

    /// Reads data from storage without taking into account byte order
    void read_raw(void * data, size_t sz, uint64_t offset = 0) const {
        storage_.read(data, sz, offset_ + offset);
    }

    /// Reads data from storage with specified offset relative to base offset
    void read(void * data, size_t sz, uint64_t offset = 0) const {
        storage_.read(data, sz, offset_ + offset);

        if (host_endianness() == end_) {
            return;
        }

        // reversing data bytes if host endianness is not equal to target
        // endianness
        unsigned char * bdata = reinterpret_cast<unsigned char*>(data);
        std::reverse(bdata, bdata + sz);
    }

    /// Writes data to storage without taking into account byte order
    void write_raw(const void * data, size_t sz, uint64_t offset = 0) const {
        storage_.write(data, sz, offset_ + offset);
    }

    /// Writes data to storage with specified offset relative to base offset
    void write(const void * data, size_t sz, uint64_t offset = 0) const {
        if (host_endianness() == end_) {
            storage_.write(data, sz, offset_ + offset);
            return;
        }

        // reversing data bytes if host endianness is not equal to target
        // endianness
        auto bdata = reinterpret_cast<const unsigned char*>(data);
        std::vector<unsigned char> reversedData(sz);
        std::copy(bdata, bdata + sz, reversedData.rbegin());
        storage_.write(&reversedData[0], sz, offset_ + offset);
    }

private:
    /// Reads value of specified type from storage
    template <typename T>
    T read_t(uint64_t offset) const;

    /// Writes value of specified type to storage
    template <typename T>
    void write_t(const T & val, uint64_t offset) const;


    storage & storage_;     ///< Reference to data storage
    endianness end_;        ///< Endianness
    uint64_t offset_;       ///< Base offset in data storage
};


}
