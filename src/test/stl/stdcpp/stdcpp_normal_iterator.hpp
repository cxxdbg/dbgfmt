
/// \file stdcpp_normal_iterator.hpp
/// Contains declaration of functions for creating types and values for libstdc++ normal iterator.

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test {


/// Gets or creates type for libstdc++ normal iterator for specified element type
cm::type_t * stdcpp_normal_iterator_type(const cm::qual_type & elt_type);

/// Creates libstdc++ normal iterator value with specified ptr
dbgfmt::value create_stdcpp_normal_iterator(const cm::qual_type & elt_type, const dbgfmt::value & ptr);


namespace tcm::stdcpp::normal_iterator {
    /// Gets or creates libstdc++ normal iterator type for specified element type
    inline cm::type_t * type(const cm::qual_type & elt_type) {
        return stdcpp_normal_iterator_type(elt_type);
    }

    /// Creates libstdc++ normal iterator value
    inline value make(const cm::qual_type & elt_type, const value & ptr) {
        return create_stdcpp_normal_iterator(elt_type, ptr);
    }
}


}
