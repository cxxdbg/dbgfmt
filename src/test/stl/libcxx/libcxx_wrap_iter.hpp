
/// \file libcxx_wrap_iter.hpp
/// Contains declaration of functions for creating libc++ std::__1::__wrap_iter types and values

#pragma once

#include "../../test2.hpp"


namespace dbgfmt::test::tcm::libcxx::wrap_iter {
    /// Gets or creats type of libc++ __wrap_iter for specified element type
    cm::type_t * type(const cm::qual_type & elt_type);

    /// Creates valie of libc++ __wrap_iter
    value make(const cm::qual_type & elt_type, const value & ptr);
}
