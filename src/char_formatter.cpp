
/// \file char_formatter.cpp
/// Contains implementations of character and string formatting functions

#include "char_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/value.hpp"
#include "dbgfmt/format_error.hpp"
#include <iostream>


namespace dbgfmt {


/// Number of max printed characters in string
constexpr size_t max_printed_chars = 100;
constexpr size_t max_chars_to_read = max_printed_chars + 1;


/// Converts vector of characters to vector of wide characters
/// using current locale. Returns iterator pointing to the first
/// input character that can't be converter.
static auto convert_chars_to_wchars(const std::vector<char> & in,
                                    std::vector<wchar_t> & out) {

    // creating multibyte -> wide converter using current locale
    std::locale loc("");
    using conv_t = std::codecvt<wchar_t, char, std::mbstate_t>;
    const conv_t & conv = std::use_facet<conv_t>(loc);

    // allocating output buffer.
    // output will take at most same number of bytes as input
    std::vector<wchar_t> res(in.size());

    // converting chars to wchars
    std::mbstate_t state = mbstate_t();
    const char * input = &in[0];
    const char * input_end = input + in.size();
    wchar_t * output = &res[0];
    const char * next_input = input;
    wchar_t * next_output = output;
    auto conv_res = conv.in(state,
                    input,
                    input_end,
                    next_input,
                    output,
                    output + res.size(),
                    next_output);

    // copying result to output vector
    out.clear();
    out.reserve(next_output - output);
    std::copy(output, next_output, std::back_inserter(out));

    return in.begin() + (next_input - input);
}


void write_formatted_string(std::wostream & str, const std::vector<char> & chars) {

    // converting chars to wchars
    std::vector<wchar_t> wchars;
    auto next_char = convert_chars_to_wchars(chars, wchars);

    if (next_char != chars.end() && wchars.size() < 101) {
        // append unconverted characters at the end
        // using raw char -> wchar_t conversion
        while (next_char != chars.end() && wchars.size() < 101) {
            wchars.push_back(static_cast<unsigned char>(*next_char));
            ++next_char;
        }
    }

    // writing wstring to output stream
    write_formatted_string(str, wchars);
}


void write_formatted_string(std::wostream & str, const std::vector<wchar_t> & chars) {

    str << "\"";

    auto sz = chars.size();
    if (sz > 100) {
        sz = 100;
    }

    for (size_t i = 0; i < sz; ++i) {
        write_formatted_char(str, chars[i]);
    }

    if (sz != chars.size()) {
        str << "...";
    }

    str << "\"";
}


/// Returns max number of characters to represent wide char
static size_t chars_per_wide_char(char) {
    std::locale loc("");
    using conv_t = std::codecvt<wchar_t, char, std::mbstate_t>;
    const conv_t & conv = std::use_facet<conv_t>(loc);
    return static_cast<size_t>(conv.max_length());
}


/// Returns max number of characters to represent wide char
static size_t chars_per_wide_char(wchar_t) {
    return 1;
}


size_t min_num_of_chars_to_format_string(char) {
    return max_chars_to_read * chars_per_wide_char(char());
}


size_t min_num_of_chars_to_format_string(wchar_t) {
    return max_chars_to_read * chars_per_wide_char(wchar_t());
}


/// Writes formatted null terminated string of values
template <typename Char>
void do_write_formatted_zstring(std::wostream & str, const value & ptr) {
    try {
        const size_t nchars = max_chars_to_read * chars_per_wide_char(Char());

        // reading characters

        std::vector<Char> chars;
        chars.reserve(nchars);

        for (size_t i = 0; i < nchars; ++i) {
            value ch = *(ptr + i);
            auto ch_val = ch.as_uint64();

            if (ch_val == 0) {
                break;
            }

            chars.push_back(static_cast<Char>(ch.as_uint64()));
        }

        // formatting characters as string
        write_formatted_string(str, chars);
    }
    catch (const format_error & err) {
        str << "<error: " << err.what() << ">";
    }
}


void write_formatted_zstring(std::wostream & str, const value & ptr) {
    do_write_formatted_zstring<char>(str, ptr);
}


void write_formatted_zwstring(std::wostream & str, const value & ptr) {
    do_write_formatted_zstring<wchar_t>(str, ptr);
}


/// Writes formatted character and its numeric representation to output stream
template <typename C>
static void format_char(const value & val, std::wostream & str) {
    auto ch = static_cast<C>(val.as_uint64());
    str << "'";
    write_formatted_char(str, ch);
    str << "' (" << static_cast<int>(ch) << ")";
}


/// Writes formatted character and its hex numeric representation to output stream
template <typename C>
static void format_char_hex(const value & val, std::wostream & str) {
    auto ch = static_cast<C>(val.as_uint64());
    str << "'";
    write_formatted_char(str, ch);
    str << "' (";
    print_hex(str, ch, sizeof(C), 1);
    str << ")";
}


void char_formatter::write(const value & val, std::wostream & str) const {
    if (ctx().hex()) {
        format_char_hex<char>(val, str);
    } else {
        format_char<char>(val, str);
    }
}


void char_formatter::hex_write(const value & val, std::wostream & str) const {
    write(val, str);
}


void wchar_formatter::write(const value & val, std::wostream & str) const {
    if (ctx().hex()) {
        format_char_hex<wchar_t>(val, str);
    } else {
        format_char<wchar_t>(val, str);
    }
}


void wchar_formatter::hex_write(const value & val, std::wostream & str) const {
    write(val, str);
}


}
