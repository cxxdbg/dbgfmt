
/// \file libcxx_ns_utils.cpp
/// Contains implementations of utility functions for working with libc++ namespace.

#include "libcxx_ns_utils.hpp"
#include "../../utils.hpp"
#include <sstream>


namespace dbgfmt {


static const std::vector<std::string> libcxx_ns_names = {"std::__1", "std::__ndk1"};


bool is_libcxx_record(const value & val, const std::string & cls_name) {
    if (!val.is_record()) {
        return false;
    }

    auto t_name = val.template_name();
    for (const auto & ns_name : libcxx_ns_names) {
        if (t_name == ns_name + "::" + cls_name) {
            return true;
        }
    }

    return false;
}


std::wstring replace_libcxx_type_name(const value & val) {
    auto res = val.wqual_type_name();
    for (const auto & ns_name : libcxx_ns_names) {
        std::wstring wn{ns_name.begin(), ns_name.end()};
        res = replace(res, wn + L"::", L"std::");
    }

    return res;
}


std::string make_libcxx_type_name(const std::string & cls_name, const value & other_val) {
    std::string res;

    // detecting libc++ namespace name from value type
    auto t_name = other_val.template_name();
    for (const auto & ns_name : libcxx_ns_names) {
        if (t_name.substr(0, ns_name.size()) == ns_name) {
            res = ns_name;
            break;
        }
    }

    if (res.empty()) {
        std::ostringstream msg;
        msg << "can't detect libc++ namespace name from type name '" << t_name << "'";
    }

    return res + "::" + cls_name;
}


}
