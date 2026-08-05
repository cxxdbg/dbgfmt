
/// \file std_vector_utils.hpp
/// Contains definition of utility functions for creating vector values

#pragma once

#include "../test2.hpp"


namespace dbgfmt::test {


/// Makes tuple of begin / end / capcacity end values for vector
inline std::tuple<value, value> make_vector_begin_end_tuple(const cm::qual_type & elt_type, const std::vector<value> & vals) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto elt_ptr_type = cm.get_or_create_ptr_type(elt_type);

    if (vals.empty()) {
        auto null = make_null_to(elt_type);
        return {null, null};
    } else {
        auto arr = make_arr(elt_type, vals.size());
        for (size_t i = 0, e = vals.size(); i < e; ++i) {
            arr[i] << vals[i];
        }

        auto begin = arr[0].ptr();
        auto end = arr[vals.size() - 1].ptr() + 1;
        return {begin, end};
    }
}


}
