
/// \file std_shared_ptr_formatter.hpp
/// Contains definition of the std_shared_ptr_formatter class.

#pragma once

#include "../smart_ptr_formatter.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ shared/weak ptr
class std_shared_ptr_formatter: virtual public smart_ptr_formatter {
private:
    /// Returns number of extra childs which should be displayed by formatter.
    /// Returns 2.
    std::size_t extra_childs_size() const override;

    /// Returns name of additional child with specified index.
    std::wstring extra_child_name(size_t idx) const override;

    /// Returns additional child with specified index.
    /// Returns use count for index 0, and weak count for index 1
    fmt_result_ref extra_child(const value & v, std::size_t index) const override;

    /// Returns use count value
    virtual uint64_t get_use_count(const value & v) const = 0;

    /// Returns weak count value
    virtual uint64_t get_weak_count(const value & v) const = 0;
};


}
