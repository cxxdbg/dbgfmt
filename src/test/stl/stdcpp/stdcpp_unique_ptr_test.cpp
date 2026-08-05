
/// \file stdcpp_unique_ptr_test.cpp
/// Contains unit tests for libstdc++ unique_ptr formatter.

#include "stdcpp_tuple.hpp"
#include "../../test2.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Makes stdcpp unique_ptr record value with specified pointer value
static value make_unique_ptr(const value & ptr) {
    auto ptr_type = val_type(ptr);
    auto ptr_ptype = ptr_type.cast<cm::pointer_type>();
    assert(ptr_ptype && "invalid pointer type");

    cm_builder b;
    auto unique_ptr_type = b
        .ns("std")
            .templ("unique_ptr", "T").record("uptr", ptr_ptype->base())
                .ivar("_M_t", get_stdcpp_tuple_type({val_type(ptr)}))
            .end().end()
        .end()
    .build().rtype("uptr");

    auto uptr = make_val(unique_ptr_type);
    uptr["_M_t"] << make_stdcpp_tuple(ptr);
    return uptr;
}


/// Makes new stdcpp unique_ptr record value with specified pointer value
static value make_unique_ptr_new(const value & ptr) {
    auto ptr_type = val_type(ptr);
    auto ptr_ptype = ptr_type.cast<cm::pointer_type>();
    assert(ptr_ptype && "invalid pointer type");

    cm_builder b;
    auto unique_ptr_type = b
        .ns("std")
            .templ("unique_ptr", "T").record("uptr", ptr_ptype->base())
                .ivar("_M_t", b.typeref("impl"))
            .end().end()
            .templ("__uniq_ptr_impl", "T").record("impl", ptr_ptype->base())
                .ivar("_M_t", get_stdcpp_tuple_type({val_type(ptr)}))
            .end().end()
        .end()
    .build().rtype("uptr");

    auto uptr = make_val(unique_ptr_type);
    uptr["_M_t"]["_M_t"] << make_stdcpp_tuple(ptr);
    return uptr;
}


/// Makes another new variant of unique_ptr record value
/// Makes new stdcpp unique_ptr record value with specified pointer value
static value make_unique_ptr_new2(const value & ptr) {
    auto ptr_type = val_type(ptr);
    auto ptr_ptype = ptr_type.cast<cm::pointer_type>();
    assert(ptr_ptype && "invalid pointer type");

    cm_builder b;
    auto unique_ptr_type = b
        .ns("std")
            .templ("unique_ptr", "T").record("uptr", ptr_ptype->base())
                .ivar("_M_t", b.typeref("data"))
            .end().end()
            .templ("__uniq_ptr_data", "T").record("data", ptr_ptype->base())
                .base(b.typeref("impl"))
            .end().end()
            .templ("__uniq_ptr_impl", "T").record("impl", ptr_ptype->base())
                .ivar("_M_t", get_stdcpp_tuple_type({val_type(ptr)}))
            .end().end()
        .end()
    .build().rtype("uptr");

    auto uptr = make_val(unique_ptr_type);
    uptr["_M_t"]["_M_t"] << make_stdcpp_tuple(ptr);
    return uptr;
}



BOOST_FIXTURE_TEST_SUITE(stdcpp_unique_ptr, fmt_test_fixture)


/// Tests formatting simple unique_ptr
BOOST_AUTO_TEST_CASE(simple) {
    auto val = make_int(10);
    auto uptr = make_unique_ptr(val.ptr());
    auto res = ctx.format(uptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.address()) + L" 10");
    BOOST_CHECK(res->type() == L"std::unique_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ptr = res->child_at(0);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(val.address()) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_CHECK(ptr->childs_size() == 1);
}


/// Tests formatting new unique_ptr
BOOST_AUTO_TEST_CASE(new_simple) {
    auto val = make_int(10);
    auto uptr = make_unique_ptr_new(val.ptr());
    auto res = ctx.format(uptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.address()) + L" 10");
    BOOST_CHECK(res->type() == L"std::unique_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ptr = res->child_at(0);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(val.address()) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_CHECK(ptr->childs_size() == 1);
}


/// Tests formatting new unique_ptr variant 2
BOOST_AUTO_TEST_CASE(new2_simple) {
    auto val = make_int(10);
    auto uptr = make_unique_ptr_new2(val.ptr());
    auto res = ctx.format(uptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.address()) + L" 10");
    BOOST_CHECK(res->type() == L"std::unique_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ptr = res->child_at(0);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(val.address()) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_CHECK(ptr->childs_size() == 1);
}



BOOST_AUTO_TEST_SUITE_END()


}
