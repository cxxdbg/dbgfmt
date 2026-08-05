
/// \file pointer_value.hpp
/// Contains definition of the pointer_value class.

#pragma once

#include "value.hpp"
#include <cm/array_type.hpp>
#include <cm/pointer_type.hpp>


namespace dbgfmt::test {


/// \class pointer_value
/// Builder for pointer value
class pointer_value: public value<cm::pointer_type> {
public:
    /// Constructs pointer value builder with specified address
    pointer_value(test_context & ctx, const qual_type & type, uint64_t addr):
    value<cm::pointer_type>(ctx, type) {
        data().write_unsigned(addr, ctx.dbg().ptr_size());
    }

    /// Constructs pointer value builder with specified address
    pointer_value(const qual_type & type, uint64_t addr):
        pointer_value(test_context::current_context(), type, addr) {}

    /// Constructs builder for pointer value with specified type and data
    pointer_value(test_context & ctx, const qual_type & type, const data::accessor & d):
        value<cm::pointer_type>(ctx, type, d) {}

    /// Constructs builder for pointer value with specified type and data
    pointer_value(const qual_type & type, const data::accessor & d):
        pointer_value(test_context::current_context(), type, d) {}

    /// Constructs pointer value builder from another value builder
    pointer_value(const value<> & val):
        pointer_value(val.type().cast<cm::pointer_type>(), val.data()) {}

    /// Returns value of pointer
    uint64_t as_uint64() const {
        return data().read_unsigned(ctx().dbg().ptr_size());
    }

    /// Constructs pointer value with offset from current pointer
    pointer_value operator+(unsigned long offs) const {
        return pointer_value{ctx(), type(), as_uint64() + offs};
    }
};


/// \class pointer_value
/// Builder for member pointer value
class mem_ptr_value: public value<cm::mem_ptr_type> {
public:
    /// Constructs pointer value builder with specified address
    mem_ptr_value(test_context & ctx,
                  const cm::qual_type_t<cm::mem_ptr_type> & type,
                  uint64_t addr):
    value<cm::mem_ptr_type>(ctx, type) {
        data().write_unsigned(addr, ctx.dbg().ptr_size());
    }

    /// Constructs pointer value builder with specified address
    mem_ptr_value(const cm::qual_type_t<cm::mem_ptr_type> & type, uint64_t addr):
        mem_ptr_value(test_context::current_context(), type, addr) {}
};


/// Returns pointer to value
template <typename Type>
pointer_value value<Type>::ptr() const {
    cm::qual_type pointee_type;
    if (auto arr_type = type().template cast<cm::array_type>()) {
        pointee_type = arr_type.type()->base();
    } else {
        pointee_type = type();
    }

    auto ptr_type = ctx().cm().get_or_create_ptr_type(pointee_type);
    return pointer_value{ctx(), ptr_type, addr()};
}


/// Returns pointer to the end of value
template <typename Type>
pointer_value value<Type>::end_ptr() const {
    return ptr() + size();
}


template <>
struct cm_type_traits<cm::pointer_type> {
    using value_type = pointer_value;
};


}


