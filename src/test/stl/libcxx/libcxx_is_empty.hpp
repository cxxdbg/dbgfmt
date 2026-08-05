
/// \file libcxx_is_empty.hpp
/// Contains definition of utility function for checking is_empty predicate for libc++ types.

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test {


/// Returns true if type is empty record value
inline bool libcxx_is_empty(const cm::qual_type & type) {
    auto rtype = type.cast<cm::record_type>();
    if (!rtype) {
        return false;
    }

    // checking if type has fields
    if (!std::ranges::empty(rtype->fields())) {
        return false;
    }

    // checking base classes
    for (auto base : rtype->bases()) {
        if (!libcxx_is_empty(base)) {
            return false;
        }
    }

    return true;
}


}
