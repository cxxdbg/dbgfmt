
/// \file utils.hpp
/// Contains definitions of utility formatting functions

#pragma once

#include "dbgfmt/value.hpp"
#include <string>


namespace dbgfmt {


class context;


/// Returns name of global object to that pointer value points to,
/// and offset to that object.
/// Tries use associated function name or symbol name.
/// Returns empty string if name is not available
std::tuple<std::wstring, uint64_t> extract_pointee_name(const value & val);


/// Writes &<pointee name> + offset to output stream
void write_pointee_name(context & ctx,
                        std::wostream & str,
                        const std::wstring & name,
                        uint64_t offset,
                        bool is_hex);


/// Replaces all substrings in string
std::wstring replace(const std::wstring & str, const std::wstring & sub_str, const std::wstring & replace_str);


}


