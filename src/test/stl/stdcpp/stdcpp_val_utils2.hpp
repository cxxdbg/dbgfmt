
/// \file stdcpp_val_utils2.hpp
/// Contains declarations of utility functions for working with new test values for libstdc++.

#pragma once

#include "stdcpp_utils.hpp"
#include "../../test2.hpp"


namespace dbgfmt::test {


/// Creates pair value
inline value make_pair(const value & first, const value & second) {
    auto res = make_val(get_stdcpp_pair_type(val_type(first), val_type(second)));
    res["first"] << first;
    res["second"] << second;
    return res;
}



}
