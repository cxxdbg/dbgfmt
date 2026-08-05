
/// \file stdcpp_val_utils.hpp
/// Contains declarations of utility functions for working with old test values for libstdc++

#pragma once

#include "stdcpp_utils.hpp"
#include "../../record_value.hpp"


namespace dbgfmt::test {


record_value make_pair(const value<> & first, const value<> & second) {
    record_value res{get_stdcpp_pair_type(first.type(), second.type())};
    res["first"] = first;
    res["second"] = second;
    return res;
}


}