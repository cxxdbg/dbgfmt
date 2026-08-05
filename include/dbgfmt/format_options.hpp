
/// \file format_options.hpp
/// Contains definition of the format_options class.

#pragma once

#include <cstddef>


namespace dbgfmt {


/// Formatting options
class format_options {
public:
    /// Constructs default formatting options
    format_options() = default;

    /// Default copy constructor
    format_options(const format_options &) = default;

    /// Default move constructor
    format_options(format_options &&) = default;

    /// Default copy assignment operator
    format_options & operator=(const format_options &) = default;

    /// Default move assignment operator
    format_options & operator=(format_options &&) = default;

    /// Returns true if raw data field should be displayed
    bool show_raw_data() const { return show_raw_data_; }

    /// Sets whether raw data field should be displayed
    void set_show_raw_data(bool v) { show_raw_data_ = v; }

    /// Creates new formatting options from this options with specified value of raw data flag
    format_options with_show_raw_data(bool val) const {
        auto res = *this;
        res.set_show_raw_data(val);
        return res;
    }

    /// Returns "display hex" flag value
    bool hex() const { return hex_; }

    /// Sets "display hex" flag value
    void set_hex(bool val) { hex_ = val; }

    /// Returns minimum number of bytes displayed for hex values
    size_t min_hex_size() const { return min_hex_size_; }

    /// Sets minimum number of bytes displayed for hex values
    void set_min_hex_size(size_t sz) { min_hex_size_ = sz; }

    /// Returns true if pointer addresses should be displayed
    bool show_ptr_addr() const { return show_ptr_addr_; }

    /// Sets whether pointer addresses should be displayed
    void set_show_ptr_addr(bool val) { show_ptr_addr_ = val; }

    /// Creates new formatting options from this options with specified value of show ptr addresses flag
    format_options with_show_ptr_addr(bool val) const {
        auto res = *this;
        res.set_show_ptr_addr(val);
        return res;
    }

    /// Returns true if record addresses should be displayed
    bool show_rec_addr() const { return show_rec_addr_; }

    /// Sets whether record addresses should be displayed
    void set_show_rec_addr(bool val) { show_rec_addr_ = val; }

    /// Returns true if records should be formatted as functors
    bool rec_as_functor() const { return rec_as_functor_; }

    /// Sets whether fecords should be formatted as functors
    void set_rec_as_functor(bool val) { rec_as_functor_ = val; }

    /// Constructs new formatting options from current options with specified value of records as functors flag
    format_options with_rec_as_functor(bool val) const {
        auto res = *this;
        res.set_rec_as_functor(val);
        return res;
    }

private:
    bool show_raw_data_ = true;     ///< Should the '[Raw Data]' field be displayed
    bool hex_ = false;              ///< Display integer values as hex
    size_t min_hex_size_ = 4;       ///< Minimum number of bytes for hex values
    bool show_ptr_addr_ = true;     ///< Should pointer addresses be displayed?
    bool show_rec_addr_ = false;    ///< Should record addresses be displayed?
    bool rec_as_functor_ = false;   ///< Format records as functors
};


/// Compares two format options instnaces
inline bool operator==(const format_options & o1, const format_options & o2) {
    return o1.show_raw_data() == o2.show_raw_data() &&
           o1.hex() == o2.hex() &&
           o1.min_hex_size() == o2.min_hex_size() &&
           o1.show_ptr_addr() == o2.show_ptr_addr() &&
           o1.show_rec_addr() == o2.show_rec_addr();
}


/// Returns true if two instances of format options are not equal
inline bool operator!=(const format_options & o1, const format_options & o2) {
    return !(o1 == o2);
}


}
