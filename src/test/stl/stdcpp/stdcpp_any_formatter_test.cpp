
/// \file stdcpp_any_formatter_test.cpp
/// Contains unit tests for libstdc++ any formatter

#include "stdcpp_utils.hpp"
#include "../../builtin_value.hpp"
#include "../../cm_builder.hpp"
#include "../../fmt_test_fixture.hpp"
#include "../../pointer_value.hpp"
#include "../../record_value.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Creates CM for testing std::any
static cm::builder_result create_any_cm(const cm::qual_type & val_type) {
    cm_builder b;

    auto manage_ftype = b.ftype(b.bt_void(),
                                b.bt_int(),
                                b.typeref("any"),
                                b.typeref("any"));

    return b
        .ns("std")
            .record("any", "any")
                .templ("_Manager_internal", "T").record("manager_internal", val_type)
                    .func("_S_manage", manage_ftype)
                .end().end()

                .templ("_Manager_external", "T").record("manager_external", val_type)
                    .func("_S_manage", manage_ftype)
                .end().end()

                .record("_Storage", "storage")
                    .ivar("_M_ptr", b.ptype(b.bt_void()))
                    .ivar("_M_buffer", get_stdcpp_aligned_storage_type(8))
                .end()

                .ivar("_M_manager", b.ptype(manage_ftype))
                .ivar("_M_storage", b.typeref("storage"))
            .end()
        .end()
    .build();
}


BOOST_FIXTURE_TEST_SUITE(stdcpp_any_formatter_test, fmt_test_fixture)


/// Tests formatting empty any value
BOOST_AUTO_TEST_CASE(empty) {
    auto acm = create_any_cm(cm.bt_int());

    record_value any_val{acm.rtype("any")};
    any_val["_M_manager"] = (intptr_t)0;

    auto res = ctx.format(any_val);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = <empty>]");
    BOOST_CHECK(res->type() == L"std::any");
    BOOST_CHECK_EQUAL(res->childs_size(), 1);

    auto res_val = res->child_at(0);
    BOOST_CHECK(res_val->name() == L"[value]");
    BOOST_CHECK(res_val->val() == L"<empty>");
    BOOST_CHECK(res_val->type() == L"");
    BOOST_CHECK_EQUAL(res_val->childs_size(), 0);
}


/// Tests formatting any value containing int
BOOST_AUTO_TEST_CASE(int_val) {
    auto acm = create_any_cm(cm.bt_int());

    // getting manage function in CM
    auto manager_type = acm.rtype("manager_internal");
    auto m_manager = dynamic_cast<cm::function*>(manager_type->find_named_entity("_S_manage"));
    BOOST_REQUIRE(m_manager != nullptr);

    // defining address of manager function
    dbg.add_object(m_manager, 0xAA, 100);

    record_value any_val{acm.rtype("any")};
    any_val["_M_manager"] = long_value{0xAA};
    any_val["_M_storage"]["_M_buffer"]["__data"] = long_value{232};
    any_val["_M_storage"]["_M_ptr"] = long_value{232};

    auto res = ctx.format(any_val);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = 232]");
    BOOST_CHECK(res->type() == L"std::any");
    BOOST_CHECK_EQUAL(res->childs_size(), 1);

    auto res_val = res->child_at(0);
    BOOST_CHECK(res_val->name() == L"[value]");
    BOOST_CHECK(res_val->val() == L"232");
    BOOST_CHECK(res_val->type() == L"int");
    BOOST_CHECK_EQUAL(res_val->childs_size(), 0);
}


/// Tests formatting any value containing record
BOOST_AUTO_TEST_CASE(rec_val) {
    cm_builder b;
    auto bres = b
        .record("myrec", "myrec")
            .ivar("x", cm.bt_int())
            .ivar("y", cm.bt_int())
        .end()
    .build();
    auto myrec_type = bres.rtype("myrec");

    auto acm = create_any_cm(myrec_type);

    // getting manage function in CM
    auto manager_type = acm.rtype("manager_external");
    auto m_manager = dynamic_cast<cm::function*>(manager_type->find_named_entity("_S_manage"));
    BOOST_REQUIRE(m_manager != nullptr);

    // defining address of manager function
    dbg.add_object(m_manager, 0xAA, 100);

    record_value myrec{myrec_type};
    myrec["x"] = 10;
    myrec["y"] = 20;

    record_value any_val{acm.rtype("any")};
    any_val["_M_manager"] = long_value{0xAA};
    any_val["_M_storage"]["_M_buffer"]["__data"] = myrec.ptr();
    any_val["_M_storage"]["_M_ptr"] = myrec.ptr();

    auto res = ctx.format(any_val);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[value = { x=10 y=20 }]");
    BOOST_CHECK(res->type() == L"std::any");
    BOOST_CHECK_EQUAL(res->childs_size(), 1);

    auto res_val = res->child_at(0);
    BOOST_CHECK(res_val->name() == L"[value]");
    BOOST_CHECK(res_val->val() == L"{ x=10 y=20 }");
    BOOST_CHECK(res_val->type() == L"myrec");
    BOOST_CHECK_EQUAL(res_val->childs_size(), 2);
}


BOOST_AUTO_TEST_SUITE_END()


}