
/// \file array_value.hpp
/// Contains definition of the array_value class.

#pragma once

#include "value.hpp"
#include <string>


namespace dbgfmt::test {


/// \class array_value
/// Builder of array values in test framework
class array_value: public value<cm::array_type> {
public:
    /// Constructs array value builder for specified array type
    array_value(test_context & ctx, const cm::qual_type_t<cm::array_type> & type):
        value(ctx, type) {}

    /// Constructs array value builder for specified array type
    array_value(const cm::qual_type_t<cm::array_type> & type):
        array_value(test_context::current_context(), type) {}

    /// Constructs array value builder for specified base type and size
    array_value(test_context & ctx, cm::type_t * base, uint64_t size):
        value(ctx, ctx.cm().get_or_create_arr_type(base, size)) {}

    /// Constructs array value builder for specified base type and size
    array_value(cm::type_t * base, uint64_t size):
        array_value(test_context::current_context(), base, size) {}

    /// Constructs array value build with specified data
    array_value(test_context & ctx, const qual_type & type, const data::accessor & d):
        value(ctx, type, d) {}

    /// Constructs array value build with specified data
    array_value(const qual_type & type, const data::accessor & d):
        value(test_context::current_context(), type, d) {}

    /// Returns value builder for array element
    value<> operator[](uint64_t idx) const {
        auto elt_sz = ctx().dbg().type_size(type()->base());
        auto offset = static_cast<int64_t>(elt_sz * idx);
        return value<>{ctx(), type()->base(), data::accessor{data(), offset}};
    }
};


template <>
struct cm_type_traits<cm::array_type> {
    using value_type = array_value;
};


template <typename T>
value<> value<T>::operator[](size_t arr_idx) const {
    return as<array_value>()[arr_idx];
}


}


