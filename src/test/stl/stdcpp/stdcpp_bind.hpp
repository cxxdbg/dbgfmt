
/// \file stdcpp_bind.hpp
/// Contains declaration of functions for creating stdc++ bind results

#pragma once

#include "stdcpp_tuple.hpp"
#include "../../test2.hpp"


namespace dbgfmt::test {


/// Gets or creates std::_Placeholder<> type with specified value
cm::record_type * get_stdcpp_placeholder_type(const cm::value & val);


/// Gets of creates std::_Bind type for specified functor type and
/// range of bound arguments
template <typename BoundArgs>
cm::record_type * get_stdcpp_bind_type(const cm::qual_type & functor_type, const BoundArgs & bound_args) {
    cm_builder b;
    return b
        .ns("std")
            .templ("_Bind", "F").record("bind", b.ftype_r(functor_type, bound_args))
                .ivar("_M_f", functor_type)
                .ivar("_M_bound_args", get_stdcpp_tuple_type(bound_args))
            .end().end()
        .end()
    .build().rtype("bind");
}


/// Creates value of the std::_Bind type with specified functor and range of bound arguments
template <typename BoundArguments>
value make_stdcpp_bind(const value & functor, const BoundArguments & bound_args) {
    auto val_type_fn = [](auto && v) { return val_type(v); };
    auto bound_args_types = bound_args | std::ranges::views::transform(val_type_fn);
    auto bind_type = get_stdcpp_bind_type(val_type(functor), bound_args_types);

    auto rec = make_val(bind_type);
    rec["_M_f"] << functor;
    rec["_M_bound_args"] << make_stdcpp_tuple_r(bound_args);
    return rec;
}


/// Creates value of std::_Bind type with specified functor and initializer list of bound arguments
inline value make_stdcpp_bind(const value & functor, const std::initializer_list<value> & args) {
    return make_stdcpp_bind<std::initializer_list<value>>(functor, args);
}


}


