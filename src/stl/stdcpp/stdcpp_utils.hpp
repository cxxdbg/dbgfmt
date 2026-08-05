
/// \file stdcpp_utils.hpp
/// Contains declarations of utility functions for formatting
/// libstdc++ types

#pragma once

#include "dbgfmt/value.hpp"
#include "dbgfmt/context.hpp"


namespace dbgfmt {


/// Returns pointer to value stored in libstdc++
/// __gnu_cxx::__aligned_buffer or __gnu_cxx::__aligned_membuf
value stdcpp_aligned_memory_ptr(context & ctx, const value & val, const std::string & tname);


}


