
/// \file libcxx_ns_utils.hpp
/// Contains declarations of utility functions for working with libc++ namespace.

#pragma once

#include "dbgfmt/value.hpp"


namespace dbgfmt {


/// Returns true if value is record with specified class name in libc++ namespace
bool is_libcxx_record(const value & val, const std::string & cls_name);


/// Replaces real libc++ class name with synthetic standard std:: name
std::wstring replace_libcxx_type_name(const value & val);


/// Makes libc++ class name with specified class name and other value from
/// libc++ namespace
std::string make_libcxx_type_name(const std::string & cls_name, const value & other_val);


}
