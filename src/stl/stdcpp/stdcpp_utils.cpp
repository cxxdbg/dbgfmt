
/// \file stdcpp_utils.cpp
/// Contains implementations of utility functions for formatting
/// libstdc++ types

#include "stdcpp_utils.hpp"
#include "dbgfmt/format_error.hpp"


namespace dbgfmt {


value stdcpp_aligned_memory_ptr(context & ctx,
                                const value & val,
                                const std::string & tname) {

    // trying __gnu_cxx::__aligned_buffer
    // TODO: we should get _M_storage field and check if it's a record,
    // but this does not work with current mock value objects with
    // fields casted to another types, so we can't test it
    auto type_ptr = ctx.find_type(tname).pointer();
    try {
        return val["_M_storage"].all_field_pointer("__data").cast(type_ptr);
    }
    catch (const dbgfmt::format_error & ) {
        // __gnu_cxx::__aligned_membuf
        return val.all_field_pointer("_M_storage").cast(type_ptr);
    }
}


}
