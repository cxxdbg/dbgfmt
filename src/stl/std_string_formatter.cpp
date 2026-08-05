
/// \file std_string_formatter.cpp
/// Contains implementation of the std_string_formatter class.

#include "std_string_formatter.hpp"
#include "../char_formatter.hpp"
#include "dbgfmt/context.hpp"
#include <sstream>


namespace dbgfmt {



bool std_string_formatter::has_capacity() const {
    return true;
}


std::wstring std_string_formatter::format_value(const value & val) const {
    std::wostringstream str;

    if (val.type().template_param_name(0) == "wchar_t") {
        format_string<wchar_t>(str, val);
    } else {
        format_string<char>(str, val);
    }

    return str.str();
}


template <typename Char>
void std_string_formatter::format_string(std::wostream & str, const value & val) const {
    auto max_chars_to_read = min_num_of_chars_to_format_string(Char());

    auto sz = size(val);
    if (sz > max_chars_to_read) {
        sz = max_chars_to_read;
    }

    std::vector<Char> chars;
    for (size_t i = 0; i < sz; ++i) {
        chars.push_back(static_cast<Char>(element(val, i).as_uint64()));
    }

    write_formatted_string(str, chars);
}


}
