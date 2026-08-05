
/// \file stdcpp_shared_ptr_test.cpp
/// Contains unit tests for libstdc++ shared_ptr/weak_ptr formatter

#include "../../builtin_value.hpp"
#include "../../cm_builder.hpp"
#include "../../error_value.hpp"
#include "../../fmt_test_fixture.hpp"
#include "../../pointer_value.hpp"
#include "../../record_value.hpp"
#include "../../util.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Creates CM for testing shared pointer to specified type
auto shared_ptr_cm(const cm::qual_type & type) {
    cm_builder b;
    return b
        .ns("std")
            .record("_Sp_counted_base", "base")
                .ivar("_M_use_count", b.bt_int())
                .ivar("_M_weak_count", b.bt_int())
            .end()

            .record("__shared_count", "shared_count")
                .ivar("_M_pi", b.ptype(b.typeref("base")))
            .end()

            .templ("shared_ptr", "T").record("shared_ptr", type)
                .ivar("_M_ptr", b.ptype(type))
                .ivar("_M_refcount", b.typeref("shared_count"))
            .end().end()

            .templ("weak_ptr", "T").record("weak_ptr", type)
                .ivar("_M_ptr", b.ptype(type))
                .ivar("_M_refcount", b.typeref("shared_count"))
            .end().end()
        .end()
    .build();
}


/// Creates _Sp_counted_base base record with specified use count and weak count values
static record_value make_sp_counted(const cm::qual_type & type, int use, int weak) {
    auto cmres = shared_ptr_cm(type);
    record_value res{cmres.rtype("base")};
    res["_M_use_count"] = int_value{use};
    res["_M_weak_count"] = int_value{weak};
    return res;
}

#if 0

/// Makes std::__shared_count record with specified use count and weak count
/// values
static record_value make_shared_count(simple_value_context_impl & vctx,
                                      int use_count,
                                      int weak_count) {
    // making std::_Sp_counted_base
    record_value sp_cnt{vctx, "std::_Sp_counted_base"};
    sp_cnt.add_field("_M_use_count", int_value{vctx, use_count});
    sp_cnt.add_field("_M_weak_count", int_value{vctx, weak_count});

    // making std::__shared_count
    record_value scnt{vctx, "std::__shared_count"};
    scnt.add_field("_M_pi", sp_cnt.pointer());

    return scnt;
}


#endif


BOOST_FIXTURE_TEST_SUITE(stdcpp_shared_ptr, fmt_test_fixture)



/// Tests displaying simple std::shared_ptr
BOOST_AUTO_TEST_CASE(shared_ptr_simple) {
    auto cmres = shared_ptr_cm(cm.bt_int());
    record_value sp_counted = make_sp_counted(cm.bt_int(), 12, 11);

    int_value val{10};
    record_value sp{cmres.rtype("shared_ptr")};
    sp["_M_ptr"] = val.ptr();
    sp["_M_refcount"]["_M_pi"] = sp_counted.ptr();

    auto res = ctx.format(sp);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(res->type() == L"std::shared_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto ucnt = res->child_at(0);
    BOOST_REQUIRE(ucnt);
    BOOST_CHECK(ucnt->name() == L"[use count]");
    BOOST_CHECK(ucnt->val() == L"12");
    BOOST_CHECK(ucnt->type() == L"int");
    BOOST_CHECK(ucnt->childs_size() == 0);

    auto wcnt = res->child_at(1);
    BOOST_REQUIRE(wcnt);
    BOOST_CHECK(wcnt->name() == L"[weak count]");
    BOOST_CHECK(wcnt->val() == L"11");
    BOOST_CHECK(wcnt->type() == L"int");
    BOOST_CHECK(wcnt->childs_size() == 0);

    auto ptr = res->child_at(2);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_CHECK(ptr->childs_size() == 1);
}


/// Tests displaying simple std::weak_ptr
BOOST_AUTO_TEST_CASE(weak_ptr_simple) {
    auto cmres = shared_ptr_cm(cm.bt_int());
    record_value sp_counted = make_sp_counted(cm.bt_int(), 12, 11);

    int_value val{10};
    record_value wp{cmres.rtype("weak_ptr")};
    wp["_M_ptr"] = val.ptr();
    wp["_M_refcount"]["_M_pi"] = sp_counted.ptr();

    auto res = ctx.format(wp);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(res->type() == L"std::weak_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto ucnt = res->child_at(0);
    BOOST_REQUIRE(ucnt);
    BOOST_CHECK(ucnt->name() == L"[use count]");
    BOOST_CHECK(ucnt->val() == L"12");
    BOOST_CHECK(ucnt->type() == L"int");
    BOOST_CHECK(ucnt->childs_size() == 0);

    auto wcnt = res->child_at(1);
    BOOST_REQUIRE(wcnt);
    BOOST_CHECK(wcnt->name() == L"[weak count]");
    BOOST_CHECK(wcnt->val() == L"11");
    BOOST_CHECK(wcnt->type() == L"int");
    BOOST_CHECK(wcnt->childs_size() == 0);

    auto ptr = res->child_at(2);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_CHECK(ptr->childs_size() == 1);
}


/// Tests displaying null std::shared_ptr
BOOST_AUTO_TEST_CASE(shared_ptr_null) {

    auto cmres = shared_ptr_cm(cm.bt_int());
    auto sp_base_ptr_type = cm.get_or_create_ptr_type(cmres.rtype("base"));

    int_value val{10};
    record_value wp{cmres.rtype("shared_ptr")};
    wp["_M_ptr"] = val.ptr();
    wp["_M_refcount"]["_M_pi"] = pointer_value{sp_base_ptr_type, 0};

    auto res = ctx.format(wp);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(res->type() == L"std::shared_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto ucnt = res->child_at(0);
    BOOST_REQUIRE(ucnt);
    BOOST_CHECK(ucnt->name() == L"[use count]");
    BOOST_CHECK(ucnt->val() == L"0");
    BOOST_CHECK(ucnt->type() == L"int");
    BOOST_CHECK_EQUAL(ucnt->childs_size(), 0);

    auto wcnt = res->child_at(1);
    BOOST_REQUIRE(wcnt);
    BOOST_CHECK(wcnt->name() == L"[weak count]");
    BOOST_CHECK(wcnt->val() == L"0");
    BOOST_CHECK(wcnt->type() == L"int");
    BOOST_CHECK_EQUAL(wcnt->childs_size(), 0);

    auto ptr = res->child_at(2);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_CHECK_EQUAL(ptr->childs_size(), 1);
}


/// Tests displaying bad std::shared_ptr
BOOST_AUTO_TEST_CASE(shared_ptr_bad) {

    auto cmres = shared_ptr_cm(cm.bt_int());
    auto sp_base_ptr_type = cm.get_or_create_ptr_type(cmres.rtype("base"));

    int_value val{10};
    record_value sp{cmres.rtype("shared_ptr")};
    sp["_M_ptr"] = val.ptr();
    sp["_M_refcount"]["_M_pi"] = error_value{cmres.rtype("base")}.ptr();

    auto res = ctx.format(sp);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(res->type() == L"std::shared_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto ucnt = res->child_at(0);
    BOOST_CHECK(is_error_res(ucnt, L"<error>", L"[use count]"));

    auto wcnt = res->child_at(1);
    BOOST_CHECK(is_error_res(wcnt, L"<error>", L"[weak count]"));

    auto ptr = res->child_at(2);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_CHECK_EQUAL(ptr->childs_size(), 1);
}


BOOST_AUTO_TEST_SUITE_END()


}
