
/// \file accessor.cpp
/// Contains implementation of the accessor class.

#include "dbgfmt/data/accessor.hpp"
#include "dbgfmt/data/error.hpp"
#include <sstream>


namespace dbgfmt::data {


uint64_t accessor::read_unsigned(size_t sz, uint64_t offset) const {
    switch (sz) {
    case 1:
        return static_cast<uint64_t>(read_t<uint8_t>(offset));
    case 2:
        return static_cast<uint64_t>(read_t<uint16_t>(offset));
    case 4:
        return static_cast<uint64_t>(read_t<uint32_t>(offset));
    case 8:
        return static_cast<uint64_t>(read_t<uint64_t>(offset));
    default: {
        std::ostringstream msg;
        msg << "Don't know how to read unsigned of size " << sz;
        throw error{msg.str()};
    }
    }
}


int64_t accessor::read_signed(size_t sz, uint64_t offset) const {
    switch (sz) {
    case 1:
        return static_cast<int64_t>(read_t<int8_t>(offset));
    case 2:
        return static_cast<int64_t>(read_t<int16_t>(offset));
    case 4:
        return static_cast<int64_t>(read_t<int32_t>(offset));
    case 8:
        return static_cast<int64_t>(read_t<int64_t>(offset));
    default: {
        std::ostringstream msg;
        msg << "Don't know how to read signed of size " << sz;
        throw error{msg.str()};
    }
    }
}


long double accessor::read_float(size_t sz, uint64_t offset) const {
    switch (sz) {
    case sizeof(float):
        return static_cast<long double>(read_t<float>(offset));
    case sizeof(double):
        return static_cast<long double>(read_t<double>(offset));
#if !defined(__APPLE__) || !defined(__arm64__)
    case sizeof(long double):
        return static_cast<long double>(read_t<long double>(offset));
#endif
    default: {
        std::ostringstream msg;
        msg << "Don't know how to read floating point of size " << sz;
        throw error{msg.str()};
    }
    }
}


int64_t accessor::read_signed_bits(size_t bits_sz,
                                   uint64_t byte_offset,
                                   unsigned int bit_offset) const {
    // reading unsigned bits
    uint64_t ubits = read_unsigned_bits(bits_sz, byte_offset, bit_offset);

    // getting sign bit
    uint64_t sign_mask = 1;
    sign_mask <<= (bits_sz - 1);
    bool is_neg = (ubits & sign_mask) != 0;

    if (!is_neg) {
        return static_cast<int64_t>(ubits);
    }

    // making sign extension
    uint64_t sign_ext = 1;
    sign_ext <<= (sizeof(uint64_t) * 8 - bits_sz);
    sign_ext -= 1;
    sign_ext <<= bits_sz;

    return static_cast<int64_t>(sign_ext | ubits);
}


uint64_t accessor::read_unsigned_bits(size_t bits_sz,
                                      uint64_t byte_offset,
                                      unsigned int bit_offset) const {
    assert(bit_offset < 8 && "invalid bit offset to read");

    // calculating number of bits in first byte
    auto first_byte_sz = 8U - bit_offset;
    if (first_byte_sz > bits_sz) {
        first_byte_sz = bits_sz;
    }

    // calculating number of bytes to read
    uint64_t bytes_sz = 1;
    bytes_sz += (bits_sz - first_byte_sz) / 8;
    if ((bits_sz - first_byte_sz) % 8 != 0) {
        bytes_sz += 1;
    }

    // calculating number of full bytes in the middle
    uint64_t middle_sz = 0;
    if (bytes_sz != 1) {
        middle_sz = bytes_sz - 1;
        if ((bits_sz - (8 - bit_offset)) % 8 != 0) {
            --middle_sz;
        }
    }

    uint64_t res = 0;
    unsigned char byte = 0;

    // reading first byte

    read_raw(&byte, 1, byte_offset);
    res = byte;

    {
        uint64_t mask = 1;
        mask <<= first_byte_sz;
        mask -= 1;
        res >>= bit_offset;
        res &= mask;
    }

    unsigned int curr_byte_offset = (endian() == endianness::little) ?
                                    0 :
                                    bits_sz - first_byte_sz;
    res <<= curr_byte_offset;

    if (endian() == endianness::little) {
        curr_byte_offset += first_byte_sz;
    } else {
        if (curr_byte_offset > 8) {
            curr_byte_offset -= 8;
        } else {
            curr_byte_offset = 0;
        }
    }


    // reading bytes in the middle

    for (size_t i = 0; i < middle_sz; ++i) {
        read_raw(&byte, 1, byte_offset + i + 1);
        uint64_t uint64_byte = byte;
        uint64_byte <<= curr_byte_offset;
        res |= uint64_byte;

        if (endian() == endianness::little) {
            curr_byte_offset += 8;
        } else {
            if (curr_byte_offset > 8) {
                curr_byte_offset -= 8;
            } else {
                curr_byte_offset = 0;
            }
        }
    }


    // reading last byte

    uint64_t last_byte_sz = 0;
    if (bits_sz > 8) {
        last_byte_sz = (bits_sz - (8 - bit_offset)) % 8;
    }

    if (last_byte_sz == 0) {
        // no last byte
        return res;
    }

    read_raw(&byte, 1, byte_offset + middle_sz + 1);

    {
        uint64_t mask = 1;
        mask <<= last_byte_sz;
        mask -= 1;

        uint64_t uint64_byte = byte;
        uint64_byte &= mask;
        uint64_byte <<= curr_byte_offset;
        res |= uint64_byte;
    }

    return res;
}


void accessor::write_unsigned(uint64_t val, size_t sz, uint64_t offset) {
    switch (sz) {
    case 1:
        write_t(static_cast<uint8_t>(val), offset);
        break;
    case 2:
        write_t(static_cast<uint16_t>(val), offset);
        break;
    case 4:
        write_t(static_cast<uint32_t>(val), offset);
        break;
    case 8:
        write_t(static_cast<uint64_t>(val), offset);
        break;
    default: {
        std::ostringstream msg;
        msg << "Don't know how to write unsigned of size " << sz;
        throw error{msg.str()};
    }
    }
}


void accessor::write_signed(int64_t val, size_t sz, uint64_t offset) {
    switch (sz) {
    case 1:
        write_t(static_cast<int8_t>(val), offset);
        break;
    case 2:
        write_t(static_cast<int16_t>(val), offset);
        break;
    case 4:
        write_t(static_cast<int32_t>(val), offset);
        break;
    case 8:
        write_t(static_cast<int64_t>(val), offset);
        break;
    default: {
        std::ostringstream msg;
        msg << "Don't know how to write unsigned of size " << sz;
        throw error{msg.str()};
    }
    }
}


void accessor::write_float(long double val, size_t sz, uint64_t offset) {
    switch (sz) {
    case sizeof(float):
        write_t(static_cast<float>(val), offset);
        break;
    case sizeof(double):
        write_t(static_cast<double>(val), offset);
        break;
#if !defined(__APPLE__) || !defined(__arm64__)
    case sizeof(long double):
        write_t(static_cast<long double>(val), offset);
        break;
#endif
    default: {
        std::ostringstream msg;
        msg << "Don't know how to write floating point of size " << sz;
        throw error{msg.str()};
    }
    }
}


template <typename T>
T accessor::read_t(uint64_t offset) const {
    T val;
    read(&val, sizeof(val), offset);
    return val;
}


template <typename T>
void accessor::write_t(const T & val, uint64_t offset) const {
    write(&val, sizeof(val), offset);
}


}
