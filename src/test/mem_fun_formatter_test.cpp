
/// \file mem_fun_formatter_test.cpp
/// Contains unit tests for member function formatter

#include "test2.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(mem_fun_formatter_test, fmt_test_fixture)


/// Tests formatting of pointer to function
BOOST_AUTO_TEST_CASE(test_mem_fun_ptr) {
    auto cls_type = cm.create_named_record("cls");

    auto func = cls_type->create_method("foo");
    func->set_ret_type(cm.bt_void());
    func->add_param(cm.bt_int());
    func->add_param(cm.bt_float());
    dbg.add_object(func, 0xA0, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_mem_ptr_type(cls_type, ftype);

    auto void_ptr_type = vctx.get_or_create_type(cm.get_or_create_ptr_type(cm.bt_void()));
    auto fptr = make_val(fptr_type);
    fptr.cast(void_ptr_type).write_uint64(0xAA);

    auto res = ctx.format(fptr);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"0x00000000000000AA &cls::foo + 10");
    BOOST_CHECK(res->type() == L"void (cls::*)(int, float)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting of pointer to function in hex mode
BOOST_AUTO_TEST_CASE(test_mem_fun_ptr_hex) {
    auto cls_type = cm.create_named_record("cls");

    auto func = cls_type->create_method("foo");
    func->set_ret_type(cm.bt_void());
    func->add_param(cm.bt_int());
    func->add_param(cm.bt_float());
    dbg.add_object(func, 0xA0, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_mem_ptr_type(cls_type, ftype);

    auto void_ptr_type = vctx.get_or_create_type(cm.get_or_create_ptr_type(cm.bt_void()));
    auto fptr = make_val(fptr_type);
    fptr.cast(void_ptr_type).write_uint64(0xAA);

    ctx.set_hex(true);
    auto res = ctx.format(fptr);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"0x00000000000000AA &cls::foo + 0x000000000000000A");
    BOOST_CHECK(res->type() == L"void (cls::*)(int, float)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting old representation of pointer to function (anon struct with two fields)
BOOST_AUTO_TEST_CASE(test_old) {
    auto cls_type = cm.create_named_record("cls");

    auto func = cls_type->create_method("foo");
    func->set_ret_type(cm.bt_void());
    func->add_param(cm.bt_int());
    func->add_param(cm.bt_float());
    dbg.add_object(func, 0xA0, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    auto rtype = cm.create_named_record("(anonymous struct)");
    rtype->create_field("__pfn", fptr_type);
    rtype->create_field("__delta", cm.bt_long());
    dbg.make_def_rec_layout(rtype);

    auto rec = make_val(rtype);
    rec["__pfn"] << make_ptr(fptr_type, 0xAA);
    rec["__delta"] << 0;

    auto res = ctx.format(rec);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"0x00000000000000AA &cls::foo + 10");
    BOOST_CHECK(res->type() == L"void (*)(int, float)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting old representation of pointer to function (anon struct with two fields) in hex mode
BOOST_AUTO_TEST_CASE(test_old_hex) {
    auto cls_type = cm.create_named_record("cls");

    auto func = cls_type->create_method("foo");
    func->set_ret_type(cm.bt_void());
    func->add_param(cm.bt_int());
    func->add_param(cm.bt_float());
    dbg.add_object(func, 0xA0, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    auto rtype = cm.create_named_record("(anonymous struct)");
    rtype->create_field("__pfn", fptr_type);
    rtype->create_field("__delta", cm.bt_long());
    dbg.make_def_rec_layout(rtype);

    auto rec = make_val(rtype);
    rec["__pfn"] << make_ptr(fptr_type, 0xAA);
    rec["__delta"] << 0;

    ctx.set_hex(true);
    auto res = ctx.format(rec);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"0x00000000000000AA &cls::foo + 0x000000000000000A");
    BOOST_CHECK(res->type() == L"void (*)(int, float)");
    BOOST_CHECK(res->childs_size() == 0);
}


BOOST_AUTO_TEST_SUITE_END()


}
