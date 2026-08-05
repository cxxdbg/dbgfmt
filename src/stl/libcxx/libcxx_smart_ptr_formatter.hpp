
/// \file libcxx_smart_ptr_formatter.hpp
/// Contains definitions of formatters for libc++ smart pointers.

#pragma once

#include "libcxx_ns_utils.hpp"
#include "../std_shared_ptr_formatter.hpp"


namespace dbgfmt {


/// Formatter for simple libc++ smart pointers.
class libcxx_smart_ptr_formatter: virtual public smart_ptr_formatter {
public:
    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "auto_ptr") ||
               is_libcxx_record(v, "unique_ptr");
    }

private:
    /// Returns value for real pointer
    value get_pointer(const value & v) const override {
        return v["__ptr_"];
    }

    /// Returns type name displayed for user
    std::wstring disp_type_name(const value & v) const override {
        return replace_libcxx_type_name(v);
    }
};


/// Formatter for shared libc++ smart pointers.
class libcxx_shared_ptr_formatter: public libcxx_smart_ptr_formatter, virtual public std_shared_ptr_formatter {
public:
    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "shared_ptr") ||
               is_libcxx_record(v, "weak_ptr");
    }

private:
    /// Returns use count value
    uint64_t get_use_count(const value & v) const override {
        auto cntrl = v["__cntrl_"];
        if (cntrl.as_uint64() == 0) {
            return 0;
        }

        return (*cntrl)["__shared_owners_"].as_uint64() + 1;
    }

    /// Returns weak count value
    uint64_t get_weak_count(const value & v) const override {
        auto cntrl = v["__cntrl_"];
        if (cntrl.as_uint64() == 0) {
            return 0;
        }

        return (*cntrl)["__shared_weak_owners_"].as_uint64();
    }
};


}
