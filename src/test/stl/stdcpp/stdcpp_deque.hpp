
/// \file stdcpp_deque.hpp
/// Contains declarations of utility functions for creating
/// libstdc++ deque types and objects

#pragma once

#include "../../test2.hpp"
#include <vector>


namespace dbgfmt::test {


/// Gets or creates type for libstdc++ deque iterator
cm::record_type * get_stdcpp_deque_iterator_type(const cm::qual_type & elt_type);

/// Gets or creates type for libstdc++ deque
cm::record_type * get_stdcpp_deque_type(const cm::qual_type & elt_type);

/// Creates libstdc++ deque iterator record with speicifed element type and
/// iterator values
value make_stdcpp_deque_iterator(const cm::qual_type & elt_type,
                                 const value & cur,
                                 const value & first,
                                 const value & last,
                                 const value & node);

/// Creates libstdc++ deque iterator record with specified element type
/// and iterator addresses
value make_stdcpp_deque_iterator(const cm::qual_type & elt_type,
                                 uint64_t cur,
                                 uint64_t first,
                                 uint64_t last,
                                 uint64_t node);

/// Creates invalid deque node
value make_stdcpp_deque_invalid_node(const cm::qual_type & elt_type);

/// Creates libstdc++ deque with specified start and finish iterators
value make_stdcpp_deque(const cm::qual_type & elt_type,
                        const value & map,
                        const value & map_size,
                        const value & start,
                        const value & finish);

/// Creates empty deque with specified number of nodes
value make_empty_stdcpp_deque(const cm::qual_type & elt_type, size_t node_count);

/// Creates deque from specified vector of value, starting from specified offset in nodes
value make_stdcpp_deque(const cm::qual_type & elt_type,
                        const std::vector<value> & vals,
                        size_t start);

}


