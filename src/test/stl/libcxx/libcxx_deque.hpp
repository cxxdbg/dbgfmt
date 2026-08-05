
/// \file libcxx_deque.hpp
/// Contains declarations of utility functions for testing libc++ deque.

#pragma once

#include "../../cm_builder.hpp"
#include "../../test2.hpp"


namespace dbgfmt::test {


/// Creates code model build result for testing libc++ deque
auto inline libcxx_deque_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("deque_base", "T").record("deque_base", elt_type)
            .end().end()

            .record("size_pair", "size_pair")
                .ivar("__value_", b.bt_unsigned_long())
            .end()

            .record("end_cap_pair", "end_cap_pair")
                .ivar("__value_", b.ptype(b.ptype(elt_type)))
            .end()

            .record("__split_buffer", "split_buffer")
                .ivar("__first_", b.ptype(b.ptype(elt_type)))
                .ivar("__begin_", b.ptype(b.ptype(elt_type)))
                .ivar("__end_", b.ptype(b.ptype(elt_type)))
                .ivar("__end_cap_", b.typeref("end_cap_pair"))
            .end()

            .templ("deque", "T").record("deque", elt_type)
                .base(b.typeref("deque_base"))

                .ivar("__map_", b.typeref("split_buffer"))
                .ivar("__start_", b.bt_unsigned_long())
                .ivar("__size_", b.typeref("size_pair"))
            .end().end()

            .templ("__deque_iterator", "T").record("deque_iterator", elt_type)
                .ivar("__ptr_", b.ptype(elt_type))
            .end().end()
        .end().end()
    .build();
}


/// Gets or creates deque type for specified element type
cm::qual_type get_libcxx_deque_type(const cm::qual_type & elt_type);


/// Makes empty deque value
value make_empty_libcxx_deque(const cm::qual_type & elt_type);


/// Makes deque from vector of values with specified number of first element in deque
value make_libcxx_deque(const cm::qual_type & elt_type, const std::vector<value> & vals, size_t first);


/// Makes deque iterator pointing to specified value
value make_libcxx_deque_iterator(const value & val);


}
