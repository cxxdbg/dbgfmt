
/// \file stdcpp_shared_ptr_formatter.hpp
/// Contains definition of the stdcpp_shared_ptr_formatter class.

#pragma once

#include "../std_shared_ptr_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ shared/weak ptr
class stdcpp_shared_ptr_formatter: public std_shared_ptr_formatter {
public:
    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    bool accepts(const value & v) const override {
        if (!v.is_record()) {
            return false;
        }

        return v.template_name() == "std::shared_ptr" || v.template_name() == "std::weak_ptr";
    }

private:
    /// Returns value for real pointer
    value get_pointer(const value & v) const override {
        return v["_M_ptr"];
    }

    /// Returns use count value
    uint64_t get_use_count(const value & v) const override {
        value count_rec_ptr = v["_M_refcount"]["_M_pi"];
        if (count_rec_ptr.as_uint64() == 0) {
            return 0;
        }

        return (*count_rec_ptr)["_M_use_count"].as_uint64();
    }

    /// Returns weak count value
    uint64_t get_weak_count(const value & v) const override {
        value count_rec_ptr = v["_M_refcount"]["_M_pi"];
        if (count_rec_ptr.as_uint64() == 0) {
            return 0;
        }

        return (*count_rec_ptr)["_M_weak_count"].as_uint64();
    }
};


}


