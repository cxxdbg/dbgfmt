
/// \file libcxx_string_formatter.cpp
/// Contains implementation of the libcxx_string_formatter class.

#include "libcxx_string_formatter.hpp"
#include "libcxx_ns_utils.hpp"


namespace dbgfmt {


/// Returns true if string has alternate layout
static bool string_has_alternate_layout(const value & v) {
    // libc++ std::string has two different layouts of members for which
    // different masks for capacity/size/etc should be applied.
    // Detecting layout by the name of the first field.
    return v["__r_"]["__value_"]["__l"].field_name_at(0) == "__data_";
}


/// Returns true if string is small
static bool string_is_small(const value & v) {
    uint64_t short_mask = 0;

    if (string_has_alternate_layout(v)) {
        short_mask = 0x80;
    } else {
        short_mask = 0x01;
    }

    auto sz_field = v["__r_"]["__value_"]["__s"]["__size_"].as_uint64();
    auto cap_field = v["__r_"]["__value_"]["__l"]["__cap_"].as_uint64();
    return (sz_field & short_mask) == 0;
}


bool libcxx_string_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "basic_string");
}


std::size_t libcxx_string_formatter::capacity(const value & val) const {
    uint64_t long_mask = 0;

    if (string_has_alternate_layout(val)) {
        // high bit is set
        long_mask = 1;
        long_mask <<= (val["__r_"]["__value_"]["__l"]["__cap_"].size() * 8 - 1);
    } else {
        long_mask = 1;
    }

    if (string_is_small(val)) {
        return val["__r_"]["__value_"]["__s"]["__data_"].array_size() - 1;
    } else {
        auto cap = val["__r_"]["__value_"]["__l"]["__cap_"].as_uint64();
        return static_cast<size_t>(cap & uint64_t(~long_mask));
    }
}


std::size_t libcxx_string_formatter::size(const value & val) const {
    if (string_is_small(val)) {
        auto sz = val["__r_"]["__value_"]["__s"]["__size_"].as_uint64();
        if (!string_has_alternate_layout(val)) {
            return sz >> 1;
        }

        return sz;
    } else {
        return static_cast<size_t>(val["__r_"]["__value_"]["__l"]["__size_"].as_uint64());
    }
}


value libcxx_string_formatter::element(const value & val, std::size_t index) const {
    if (string_is_small(val)) {
        return val["__r_"]["__value_"]["__s"]["__data_"][index];
    } else {
        return *(val["__r_"]["__value_"]["__l"]["__data_"] + index);
    }
}


std::wstring libcxx_string_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}



}
