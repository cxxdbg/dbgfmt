
/// \file char_formatter.hpp
/// Contains definition of classes and functions for formatting characters
/// and strings

#pragma once

#include "simple_formatter.hpp"
#include "dbgfmt/value.hpp"
#include "print.hpp"
#include <locale>
#include <ostream>
#include <vector>


namespace dbgfmt {


/// Writes formatted character to output stream
template <typename C>
void write_formatted_char(std::wostream & str, C c) {
    std::locale loc("");

    switch (c) {
    case '\r':
        str << "\\r";
        break;
    case '\n':
        str << "\\n";
        break;
    default:
        if (std::isgraph(c, loc) || std::isspace(c, loc)) {
            str << c;
        } else {
            str << "\\x";
            print_hex_no_prefix(str, c, sizeof(C), 1);
        }
    }
}


/// Formats array of characters as string. Writes at most
/// 100 characters and appends ... if number of characters is
/// greater than 100
void write_formatted_string(std::wostream & str, const std::vector<char> & chars);


/// Formats array of wide characters as string. Writes at most
/// 100 characters and appends ... if number of characters is
/// greater than 100
void write_formatted_string(std::wostream & str, const std::vector<wchar_t> & chars);


/// Returns min number of chars required to format string
size_t min_num_of_chars_to_format_string(char);


/// Returns min number of wide chars required to format string
size_t min_num_of_chars_to_format_string(wchar_t);


/// Formats null terminated string of values and writes it to output stream
void write_formatted_zstring(std::wostream & str, const value & ptr);


/// Formats null terminated wstring of values and writes it to output stream
void write_formatted_zwstring(std::wostream & str, const value & ptr);


/// Formatter for char values
class char_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override {
        return (v.is_signed() || v.is_unsigned()) && v.size() == 1;
    }

private:
    void write(const value & val, std::wostream & str) const override;
    void hex_write(const value & val, std::wostream & str) const override;
};


/// Formatter for wide characters
class wchar_formatter: public simple_formatter {
public:
    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override {
        return v.is_wchar();
    }

private:
    void write(const value & val, std::wostream & str) const override;
    void hex_write(const value & val, std::wostream & str) const override;
};


}


