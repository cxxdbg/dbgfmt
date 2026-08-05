
/// \file libcxx_bind.hpp
/// Contains definition of utility functions for making libc++ bind related values.

#pragma once

#include "libcxx_tuple.hpp"
#include "../../test2.hpp"


namespace dbgfmt::test {


/// Returns libc++ placeholder type for specified index value
cm::record_type * get_libcxx_placeholder_type(const cm::value & idx) {
    cm_builder b;
    return b
        .ns("std").ns("__1").ns("placeholders")
            .templ("__ph", std::make_tuple("I", b.bt_int())).record("placeholder", idx)
            .end().end()
        .end().end().end()
    .build().rtype("placeholder");
}


/// Gets of creates std::__1::__bind type for specified functor type and
/// range of bound arguments
template <typename BoundArgs>
cm::record_type * get_libcxx_bind_type(const cm::qual_type & functor_type, const BoundArgs & bound_args) {
    std::vector<cm::qual_type> bind_pars;
    bind_pars.push_back(functor_type);
    bind_pars.insert(bind_pars.end(), std::ranges::begin(bound_args), std::ranges::end(bound_args));

    cm_builder b;
    return b
        .ns("std").ns("__1")
            .vtempl("__bind", "Functor", "BoundArguments").record_r("bind", bind_pars)
                .ivar("__f_", functor_type)
                .ivar("__bound_args_", libcxx_tuple_type_r(bound_args))
            .end().end()
        .end().end()
    .build().rtype("bind");
}


/// Creates value of the __bind type with specified functor and range of bound arguments
template <typename BoundArguments>
value make_libcxx_bind(const value & functor, const BoundArguments & bound_args) {
    auto val_type_fn = [](auto && v) { return val_type(v); };
    auto bound_args_types = bound_args | std::ranges::views::transform(val_type_fn);
    auto bind_type = get_libcxx_bind_type(val_type(functor), bound_args_types);

    auto rec = make_val(bind_type);
    rec["__f_"] << functor;
    rec["__bound_args_"] << make_libcxx_tuple_r(bound_args);
    return rec;
}


/// Creates value of __bind type with specified functor and initializer list of bound arguments
inline value make_libcxx_bind(const value & functor, const std::initializer_list<value> & args) {
    return make_libcxx_bind<std::initializer_list<value>>(functor, args);
}


}
