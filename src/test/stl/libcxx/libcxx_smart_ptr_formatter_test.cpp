
/// \file libcxx_smart_ptr_test.cpp
/// Contains unit tests for libc++ smart pointer formatters.

#include "../../test2.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Makes code model for testing libc++ simple smart pointer formatter
static cm::builder_result make_smart_ptr_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("auto_ptr", "T").record("auto_ptr", elt_type)
                .ivar("__ptr_", b.ptype(elt_type))
            .end().end()

            .templ("unique_ptr", "T").record("unique_ptr", elt_type)
                .ivar("__ptr_", b.ptype(elt_type))
            .end().end()

            .record("__shared_count", "shared_count")
                .ivar("__shared_owners_", b.bt_unsigned_long())
            .end()

            .record("__shared_weak_count", "shared_weak_count")
                .base(b.typeref("shared_count"))
                .ivar("__shared_weak_owners_", b.bt_unsigned_long())
            .end()

            .templ("shared_ptr", "T").record("shared_ptr", elt_type)
                .ivar("__ptr_", b.ptype(elt_type))
                .ivar("__cntrl_", b.ptype(b.typeref("shared_weak_count")))
            .end().end()

            .templ("weak_ptr", "T").record("weak_ptr", elt_type)
                .ivar("__ptr_", b.ptype(elt_type))
                .ivar("__cntrl_", b.ptype(b.typeref("shared_weak_count")))
            .end().end()
        .end().end()
    .build();
}


BOOST_FIXTURE_TEST_SUITE(libcxx_smart_ptr_formatter_test, fmt_test_fixture)


/// Tests displaying simple smart pointer with specified name
void test_simple_smart_ptr(context & ctx, const std::string & ptr_name, bool is_shared) {
    auto & tctx = test_context::current_context();
    auto ptr_types = make_smart_ptr_cm(tctx.cm().bt_int());
    auto ptr_type = ptr_types.type(ptr_name);
    auto ptr = make_val(ptr_type);
    auto val = make_int(10);
    ptr["__ptr_"] << val.ptr();

    if (is_shared) {
        auto scount = make_val(ptr_types.type("shared_weak_count"));
        scount["__shared_owners_"] << 1;
        scount["__shared_weak_owners_"] << 3;
        ptr["__cntrl_"] << scount.ptr();
    }

    auto res = ctx.format(ptr);

    BOOST_CHECK(res->val() == addr_str(val.address()) + L" 10");
    std::wstring wptr_name{ptr_name.begin(), ptr_name.end()};
    std::wstring tname = L"std::";
    tname += wptr_name + L"<int>";
    BOOST_CHECK(res->type() == tname);

    BOOST_REQUIRE_EQUAL(res->childs_size(), is_shared ? 3 : 1);

    if (is_shared) {
        auto ucount = res->child_at(0);
        BOOST_CHECK(ucount->name() == L"[use count]");
        BOOST_CHECK(ucount->val() == L"2");
        BOOST_CHECK(ucount->type() == L"int");
        BOOST_CHECK_EQUAL(ucount->childs_size(), 0);

        auto wcount = res->child_at(1);
        BOOST_CHECK(wcount->name() == L"[weak count]");
        BOOST_CHECK(wcount->val() == L"3");
        BOOST_CHECK(wcount->type() == L"int");
        BOOST_CHECK_EQUAL(ucount->childs_size(), 0);
    }

    auto child = res->child_at(is_shared ? 2 : 0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"[ptr]");
    BOOST_CHECK(child->val() == addr_str(val.address()) + L" 10");
    BOOST_CHECK(child->type() == L"int*");
    BOOST_CHECK(child->childs_size() == 1);
}


/// Tests displaying simple libc++ auto_ptr
BOOST_AUTO_TEST_CASE(simple_auto_ptr) {
    test_simple_smart_ptr(ctx, "auto_ptr", false);
}


/// Tests displaying simple libc++ unique_ptr
BOOST_AUTO_TEST_CASE(simple_unique_ptr) {
    test_simple_smart_ptr(ctx, "unique_ptr", false);
}


/// Tests displaying simple libc++ shared_ptr
BOOST_AUTO_TEST_CASE(simple_shared_ptr) {
    test_simple_smart_ptr(ctx, "shared_ptr", true);
}


/// Tests displaying simple libc++ weak_ptr
BOOST_AUTO_TEST_CASE(simpl_weak_ptr) {
    test_simple_smart_ptr(ctx, "weak_ptr", true);
}


BOOST_AUTO_TEST_SUITE_END()


}
