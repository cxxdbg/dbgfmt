
/// \file util.hpp
/// Contains unitility functions for formatters tests

#pragma once

#include "../print.hpp"
#include "dbgfmt/fmt_result.hpp"
#include <sstream>


namespace dbgfmt::test {


/// Converts address to string
inline std::wstring addr_str(uint64_t addr) {
    std::ostringstream str;
    print_hex(str, addr, 8, 8);
    const auto & s = str.str();
    return std::wstring{s.begin(), s.end()};
}


inline bool starts_with(const std::wstring & str, const std::wstring & start) {
    return str.substr(0, start.size()) == start;
}


inline std::wstring skip_addr(const std::wstring & str, uint64_t addr) {
    return str.substr(addr_str(addr).size() + 1);
}


/// Returns true if string contains formatting error message in form of "<error: msg>"
inline bool is_error_msg(const std::wstring & str) {
    if (str.empty()) {
        return false;
    }

    if (!starts_with(str, L"<error: ")) {
        return false;
    }

    if (str.back() != L'>') {
        return false;
    }

    return true;
}


/// Returns true if result node contains formatting error message in form of "<error: msg>"
inline bool is_error_res(const fmt_result_ref & res, const std::wstring & type) {
    if (!res) {
        return false;
    }

    if (!is_error_msg(res->val())) {
        return false;
    }

    if (res->type() != type) {
        return false;
    }

    if (res->childs_size() != 0) {
        return false;
    }

    return true;
}


/// Returns true if named result node contains formatting error
/// message in form of "<error: msg>"
inline bool is_error_res(const named_fmt_result & res,
                         const std::wstring & type,
                         const std::wstring & nm = {}) {
    if (!res) {
        return false;
    }

    if (!is_error_res(res.res(), type)) {
        return false;
    }

    if (res.name() != nm) {
        return false;
    }

    return true;
}



}


