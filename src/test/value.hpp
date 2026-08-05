
/// \file value.hpp
/// Contains definition of the value class.

#pragma once

#include "test_context.hpp"
#include "dbgfmt/simple_value_impl.hpp"
#include "dbgfmt/value.hpp"
#include "dbgfmt/backend/forward.hpp"


namespace dbgfmt::test {


class pointer_value;


/// CM type traits for value generation framework
template <typename Type>
struct cm_type_traits;


/// Base class of all value builders
class value_base {
public:
    /// Constructs value builder with specified reference to test context,
    /// value data accessor, and location in source code
    value_base(test_context & ctx, const data::accessor & d):
        ctx_{ctx}, data_{d} {}

    /// Returns value ctx
    auto & ctx() const { return ctx_; }

    /// Returns value data accessor
    data::accessor data() const { return data_; }

private:
    test_context & ctx_;   ///< Reference to simple value context
    data::accessor data_;               ///< Value data accessor
};

/// Parameterized base class of all value builders
template <typename Type = cm::type_t>
class value: public value_base {
public:
    /// CM type
    using type_t = Type;

    /// Type of cv-qualified type from CM
    using qual_type = cm::qual_type_t<Type>;


    /// Constructs builder for new value with specified type and source location
    explicit value(test_context & ctx, const qual_type & type):
        value(ctx, type, ctx.alloc_data(type)) {}

    /// Constructs builder for new error value with specified type
    explicit value(int, test_context & ctx, const qual_type & type):
        value(ctx, type, ctx.alloc_error_data(type)) {}

    /// Constructs builder for value with specified type and data
    explicit value(test_context & ctx, const qual_type & type, const data::accessor & d):
        value_base(ctx, d), type_{type} {}

    /// Allocates memory for new value and copies data from specified value
    template <typename T>
    explicit value(const value<T> & val):
    value(val.ctx(), val.type(), val.ctx().alloc_data(val.type())) {
        *this = val;
    }

    /// Copy constructor, allocates memory for new value and copies data
    /// from specified value
    value(const value<Type> & val):
    value(val.ctx(), val.type(), val.ctx().alloc_data(val.type())) {
        *this = val;
    }

    /// Default move constructor
    value(value<Type> && val) = default;

    /// Returns value type
    auto & type() const { return type_; }

    /// Returns type size
    auto size() const { return ctx().dbg().type_size(type()); }

    /// Converts to fmt value
    operator dbgfmt::value() const {
        auto fmtcm_type = ctx().vctx().get_or_create_type(type());
        auto impl = std::make_shared<simple_value_impl>(ctx().vctx(),
                                                        fmtcm_type,
                                                        data().stor(),
                                                        data().endian(),
                                                        data().offset());
        return dbgfmt::value{impl};
    }

    /// Casts value to another type
    template <typename Type2>
    auto cast(const cm::qual_type_t<Type2> & type) const {
        using RetType = typename cm_type_traits<Type2>::value_type;
        return RetType{ctx(), type, data()};
    }

    /// Casts value to another type
    template <typename Type2>
    auto cast(Type2 * type) const {
        return cast(cm::qual_type_t<Type2>{type});
    }

    /// Converts this object to another type derived from value<> instance
    template <typename T>
    T as() const {
        auto new_type = type().template cast<typename T::type_t>();
        assert(new_type && "can't convert value type");
        return T{ctx(), new_type, data()};
    }

    /// Assigns one value to another. Copies data from memory pointed by
    /// specified value to memory pointed by this value
    template <typename Type2>
    value<Type> & operator=(const value<Type2> & v) {
        auto sz = ctx().dbg().type_size(type());
        assert(sz == ctx().dbg().type_size(v.type()) && "can't assign values of different sizes");

        // reading data from v value
        std::vector<uint8_t> vdata(sz);
        v.data().read(&vdata[0], sz);

        // writing data to this value
        data().write(&vdata[0], sz);

        return *this;
    }

    /// Assignment operator, does same as template operator=
    value<Type> & operator=(const value<Type> & v) {
        return this->operator=<Type>(v);
    }

    /// Conversion to value<>
    operator value<>() const {
        return value<>{ctx(), type(), data()};
    }

    /// Returns value address
    uint64_t addr() const {
        return data().offset();
    }

    /// Returns address pointing at the end of value
    uint64_t end_addr() const {
        return addr() + size();
    }


    pointer_value ptr() const;
    pointer_value end_ptr() const;

    value<Type> & operator=(char v);
    value<Type> & operator=(short v);
    value<Type> & operator=(int v);
    value<Type> & operator=(long v);
    value<Type> & operator=(long long v);

    value<Type> & operator=(unsigned char v);
    value<Type> & operator=(unsigned short v);
    value<Type> & operator=(unsigned int v);
    value<Type> & operator=(unsigned long v);
    value<Type> & operator=(unsigned long long v);

    value<Type> & operator=(float v);
    value<Type> & operator=(double v);
    value<Type> & operator=(long double v);

    value<Type> & operator=(wchar_t v);


    value<> operator[](const std::string & fld_name) const;
    value<> operator[](size_t arr_idx) const;

private:
    cm::qual_type_t<Type> type_;    ///< Value type
};


template <typename Type>
struct cm_type_traits {
    using value_type = value<Type>;
};


}


