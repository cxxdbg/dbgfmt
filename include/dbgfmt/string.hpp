
/// \file string.hpp
/// Contains definition of the string class.

#pragma once

#include "ref_counted.hpp"
#include <cstddef>
#include <string>


namespace dbgfmt {


/// Represents string bridge interface for external formatters and
/// unit tests
class string {
public:
    /// Returns pointer to string characters buffer
    virtual const wchar_t * data() const = 0;

    /// Returns string size
    virtual size_t size() const = 0;

    /// Reads string data and converts it to std::wstring
    std::wstring read() {
        std::wstring res;
        auto sz = size();
        res.reserve(sz);
        std::copy(data(), data() + sz, std::back_inserter(res));
        return res;
    }
};


/// Represents ref counted string bridge interface
class ref_counted_string: virtual public string, virtual public ref_counted {};


/// String bridge implementation via std::wstring
class string_impl: public std::wstring,
                   virtual public string {
public:
    /// Constructs empty string
    string_impl() = default;

    /// Constructs string from std::wstring
    string_impl(const std::wstring & str):
        std::wstring{str} {}

    /// Returns pointer to string data
    const wchar_t * data() const override {
        return std::wstring::data();
    }

    /// Returns string size
    size_t size() const override {
        return std::wstring::size();
    }
};


/// Reference counted string bridge implementation via std::wstring
class ref_counted_string_impl: public string_impl,
                               public ref_counted_impl,
                               virtual public ref_counted_string {
public:
    /// Constructs empty string
    ref_counted_string_impl() = default;

    /// Constructs string implementation from std::wstring value
    ref_counted_string_impl(const std::wstring & s): string_impl{s} {}
};


/// Reference wrapper for string
class string_ref: public ref_counted_ref_base<ref_counted_string> {
    friend string_ref acquire_string(ref_counted_string * str);

public:
    /// Default constructor
    string_ref() = default;

    /// Reads string value
    std::wstring read() const {
        return get()->read();
    }

private:
    /// Acquires reference to string. Does not change reference counter
    explicit string_ref(ref_counted_string * s): ref_counted_ref_base<ref_counted_string>{s} {}
};


/// Acuqires ownership of a string. Does not change refernce
/// counter. It should be > 0.
inline string_ref acquire_string(ref_counted_string * str) {
    return string_ref{str};
}


/// Makes ref counted string and returns reference to it
inline string_ref make_string(std::wstring val = {}) {
    auto s = std::make_unique<ref_counted_string_impl>(val);
    s->add_ref();
    return acquire_string(s.release());
}



}


