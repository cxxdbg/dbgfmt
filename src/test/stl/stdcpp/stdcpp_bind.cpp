
/// \file stdcpp_bind.cpp
/// Contains implementation of functions for creating stdc++ bind results

#include "stdcpp_bind.hpp"
#include "stdcpp_tuple.hpp"


namespace dbgfmt::test {


/// Gets or creates std::_Placeholder<> type with specified value
cm::record_type * get_stdcpp_placeholder_type(const cm::value & val) {
    cm_builder b;
    return b
        .ns("std")
            .templ("_Placeholder", std::make_tuple("N", b.bt_int()))
                .record("placeholder", val)
                .end()
            .end()
        .end()
    .build().rtype("placeholder");
}


}
