
/// \file pointer_formatter_test.cpp
/// Contains unit test for pointer formatter

#include "array_value.hpp"
#include "builtin_value.hpp"
#include "cm_builder.hpp"
#include "error_value.hpp"
#include "fmt_test_fixture.hpp"
#include "pointer_value.hpp"
#include "record_value.hpp"
#include "util.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(pointer_formatter, fmt_test_fixture)


/// Tests formatting pointer value
BOOST_AUTO_TEST_CASE(pointer_value_test) {
    int_value val{22};
    auto var = cm.create_var("my_var", cm.bt_int());
    var->set_loc({cm.source("my_test.cpp"), 400});
    dbg.add_object(var, val.addr(), dbg.type_size(var->type()));
    auto res = ctx.format(val.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.addr()) + L" 22");
    BOOST_CHECK(res->type() == L"int*");
    BOOST_CHECK(res->val_pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 400);

    BOOST_CHECK(res->childs_size() == 1);
    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"22");
    BOOST_CHECK(child->type() == L"int");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests formatting pointer value without address
BOOST_AUTO_TEST_CASE(pointer_value_test_no_addr) {
    int_value val{22};
    auto var = cm.create_var("my_var", cm.bt_int());
    var->set_loc({cm.source("my_test.cpp"), 400});
    dbg.add_object(var, val.addr(), dbg.type_size(var->type()));

    ctx.set_show_ptr_addr(false);;
    auto res = ctx.format(val.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"22");
    BOOST_CHECK(res->type() == L"int*");
    BOOST_CHECK(res->val_pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 400);

    BOOST_CHECK(res->childs_size() == 1);
    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"22");
    BOOST_CHECK(child->type() == L"int");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests formatting pointer to invalid value
BOOST_AUTO_TEST_CASE(pointer_to_error_value_test) {
    error_value val{cm.bt_int()};
    auto res = ctx.format(val.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(starts_with(res->val(), addr_str(val.addr())));
    BOOST_CHECK(is_error_msg(skip_addr(res->val(), val.addr())));
    BOOST_CHECK(res->type() == L"int*");

    BOOST_CHECK(res->childs_size() == 1);
    auto child = res->child_at(0);
    BOOST_CHECK(is_error_res(child, L"int"));
}


/// Tests reading pointer value with error
BOOST_AUTO_TEST_CASE(pointer_value_error_test) {
    auto ptr_type = cm.get_or_create_ptr_type(cm.bt_int());
    auto res = ctx.format(error_value{ptr_type});
    BOOST_CHECK(is_error_res(res, L"int*"));
}


/// Tests reading pointer which can't be dereferenced
BOOST_AUTO_TEST_CASE(pointer_value_cant_deref_test) {
    auto ftype = cm.get_or_create_func_type(cm.bt_void());
    pointer_value fptr{cm.get_or_create_ptr_type(ftype), 0x30002};
    auto res = ctx.format(fptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x0000000000030002");
    BOOST_CHECK(res->type() == L"void (*)()");
}


/// Tests formatting pointer which can't be dereferenced with addresses turned off
BOOST_AUTO_TEST_CASE(pointer_value_cant_deref_test_no_addr) {
    auto ftype = cm.get_or_create_func_type(cm.bt_void());
    pointer_value fptr{cm.get_or_create_ptr_type(ftype), 0x30002};

    ctx.set_show_ptr_addr(false);
    auto res = ctx.format(fptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x0000000000030002");
    BOOST_CHECK(res->type() == L"void (*)()");
}


/// Tests reading pointer to null terminated string
BOOST_AUTO_TEST_CASE(pointer_null_term_string_test) {
    array_value str{cm.bt_char(), 6};
    str[0] = 's';
    str[1] = ' ';
    str[2] = 't';
    str[3] = '\n';
    str[4] = 'r';
    str[5] = '\0';

    auto char_ptr_type = cm.get_or_create_ptr_type(cm.bt_char());
    auto res = ctx.format(str.ptr().cast(char_ptr_type));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(str.addr()) + L" \"s t\\nr\"");
    BOOST_CHECK(res->type() == L"char*");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"'s' (115)");
    BOOST_CHECK(child->type() == L"char");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests formatting pointer to null terminated string with no address
BOOST_AUTO_TEST_CASE(pointer_null_term_string_test_no_addr) {
    array_value str{cm.bt_char(), 6};
    str[0] = 's';
    str[1] = ' ';
    str[2] = 't';
    str[3] = '\n';
    str[4] = 'r';
    str[5] = '\0';

    auto char_ptr_type = cm.get_or_create_ptr_type(cm.bt_char());
    ctx.set_show_ptr_addr(false);
    auto res = ctx.format(str.ptr().cast(char_ptr_type));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"\"s t\\nr\"");
    BOOST_CHECK(res->type() == L"char*");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"'s' (115)");
    BOOST_CHECK(child->type() == L"char");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests reading pointer to invalid null terminated string
BOOST_AUTO_TEST_CASE(pointer_invalid_null_term_string_test) {
    array_value str{cm.bt_char(), 3};
    str[0] = 's';
    str[1] = 't';
    str[2] = 'r';

    auto char_ptr = cm.get_or_create_ptr_type(cm.bt_char());
    auto res = ctx.format(str.ptr().cast(char_ptr));

    auto astr = addr_str(str.addr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(starts_with(res->val(), astr));
    BOOST_CHECK(is_error_msg(res->val().substr(astr.size() + 1)));
    BOOST_CHECK(res->type() == L"char*");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"'s' (115)");
    BOOST_CHECK(child->type() == L"char");
    BOOST_CHECK(child->childs_size() == 0);
}


///// Tests reading pointer to big null terminated string
BOOST_AUTO_TEST_CASE(pointer_big_null_term_string_test) {
    array_value str{cm.bt_char(), 1001};
    for (size_t i = 0; i < 1000; ++i) {
        str[i] = 'x';
    }

    str[1000] = '\0';

    auto char_ptr = cm.get_or_create_ptr_type(cm.bt_char());
    auto res = ctx.format(str.ptr().cast(char_ptr));

    std::wstring res_str_value = addr_str(str.addr()) + L" \"";
    for (std::size_t i = 0; i < 100; ++i) {
        res_str_value.push_back(L'x');
    }
    res_str_value += L"...\"";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == res_str_value);
    BOOST_CHECK(res->type() == L"char*");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"'x' (120)");
    BOOST_CHECK(child->type() == L"char");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests formatting pointer to record
BOOST_AUTO_TEST_CASE(ptr_record_value_test) {
    auto rtype = cm.create_named_record("my_rec");
    auto fld = rtype->create_field("fld", cm.bt_int());
    dbg.make_def_rec_layout(rtype);

    auto rec = record_value{rtype};
    rec["fld"] = 10;

    auto res = ctx.format(rec.ptr());
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(rec.addr()) + L" { fld=10 }");
    BOOST_CHECK(res->type() == L"my_rec*");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ch = res->child_at(0);
    BOOST_REQUIRE(ch);
    BOOST_CHECK(ch->name() == L"fld");
    BOOST_CHECK(ch->val() == L"10");
    BOOST_CHECK(ch->type() == L"int");
    BOOST_CHECK(ch->childs_size() == 0);
}


/// Tests formatting pointer to record with ptr address disabled
BOOST_AUTO_TEST_CASE(ptr_record_value_test_no_ptr_addr) {
    auto rtype = cm.create_named_record("my_rec");
    auto fld = rtype->create_field("fld", cm.bt_int());
    dbg.make_def_rec_layout(rtype);

    auto rec = record_value{rtype};
    rec["fld"] = 10;

    auto opts = ctx.fmt_opts();
    opts.set_show_ptr_addr(false);
    ctx.set_fmt_opts(opts);
    auto res = ctx.format(rec.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"{ fld=10 }");
    BOOST_CHECK(res->type() == L"my_rec*");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ch = res->child_at(0);
    BOOST_REQUIRE(ch);
    BOOST_CHECK(ch->name() == L"fld");
    BOOST_CHECK(ch->val() == L"10");
    BOOST_CHECK(ch->type() == L"int");
    BOOST_CHECK(ch->childs_size() == 0);
}


/// Tests formatting pointer to record with ptr address disabled and record address enabled
BOOST_AUTO_TEST_CASE(ptr_record_value_test_no_ptr_addr_rec_addr) {
    auto rtype = cm.create_named_record("my_rec");
    auto fld = rtype->create_field("fld", cm.bt_int());
    dbg.make_def_rec_layout(rtype);

    auto rec = record_value{rtype};
    rec["fld"] = 10;

    auto opts = ctx.fmt_opts();
    opts.set_show_ptr_addr(false);
    opts.set_show_rec_addr(true);
    ctx.set_fmt_opts(opts);
    auto res = ctx.format(rec.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(rec.addr()) + L" { fld=10 }");
    BOOST_CHECK(res->type() == L"my_rec*");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ch = res->child_at(0);
    BOOST_REQUIRE(ch);
    BOOST_CHECK(ch->name() == L"fld");
    BOOST_CHECK(ch->val() == L"10");
    BOOST_CHECK(ch->type() == L"int");
    BOOST_CHECK(ch->childs_size() == 0);
}


/// Tests formatting pointer to record with ptr address enabled and record address enabled
BOOST_AUTO_TEST_CASE(ptr_record_value_test_rec_addr) {
    auto rtype = cm.create_named_record("my_rec");
    auto fld = rtype->create_field("fld", cm.bt_int());
    dbg.make_def_rec_layout(rtype);

    auto rec = record_value{rtype};
    rec["fld"] = 10;

    auto opts = ctx.fmt_opts();
    opts.set_show_rec_addr(true);
    ctx.set_fmt_opts(opts);
    auto res = ctx.format(rec.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(rec.addr()) + L" " + addr_str(rec.addr()) + L" { fld=10 }");
    BOOST_CHECK(res->type() == L"my_rec*");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ch = res->child_at(0);
    BOOST_REQUIRE(ch);
    BOOST_CHECK(ch->name() == L"fld");
    BOOST_CHECK(ch->val() == L"10");
    BOOST_CHECK(ch->type() == L"int");
    BOOST_CHECK(ch->childs_size() == 0);
}


/// Tests displaying pointer to record with dynamic type
BOOST_AUTO_TEST_CASE(ptr_record_dynamic_value_test) {
    cm_builder b;
    auto cm_res = b
        .record("base1", "base1")
            .ivar("b1_var", b.bt_int())
        .end()

        .record("base2", "base2")
            .ivar("b2_var", b.bt_int())
        .end()

        .record("derived", "derived")
            .base(b.typeref("base1"))
            .base(b.typeref("base2"))
            .ivar("d_var", b.bt_int())
        .end()
    .build();

    auto rec = record_value{cm_res.rtype("derived")};
    rec["b1_var"] = 10;
    rec["b2_var"] = 20;
    rec["d_var"] = 30;

    auto base2 = rec.base(cm_res.rtype("base2"));
    auto ptr = base2.ptr();

    auto derived_ptr = cm.get_or_create_ptr_type(cm_res.rtype("derived"));
    vctx.add_dynamic_type(base2.addr(), vctx.get_or_create_type(derived_ptr), rec.addr());

    auto res = ctx.format(ptr);
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(base2.addr()) + L" [dyn addr = " + addr_str(rec.addr()) + L"] { d_var=30 }");
    BOOST_CHECK(res->type() == L"derived*");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto base1_res = res->child_at(0);
    BOOST_REQUIRE(base1_res);
    BOOST_CHECK(base1_res->name() == L"base1");
    BOOST_CHECK(base1_res->val() == L"{ b1_var=10 }");
    BOOST_CHECK(base1_res->type() == L"base1");
    BOOST_REQUIRE_EQUAL(base1_res->childs_size(), 1);

    auto ch1 = base1_res->child_at(0);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"b1_var");
    BOOST_CHECK(ch1->val() == L"10");
    BOOST_CHECK(ch1->type() == L"int");
    BOOST_CHECK(ch1->childs_size() == 0);

    auto base2_res = res->child_at(1);
    BOOST_REQUIRE(base2_res);
    BOOST_CHECK(base2_res->name() == L"base2");
    BOOST_CHECK(base2_res->val() == L"{ b2_var=20 }");
    BOOST_CHECK(base2_res->type() == L"base2");
    BOOST_REQUIRE_EQUAL(base2_res->childs_size(), 1);

    auto ch2 = base2_res->child_at(0);
    BOOST_REQUIRE(ch2);
    BOOST_CHECK(ch2->name() == L"b2_var");
    BOOST_CHECK(ch2->val() == L"20");
    BOOST_CHECK(ch2->type() == L"int");
    BOOST_CHECK(ch2->childs_size() == 0);

    auto ch3 = res->child_at(2);
    BOOST_REQUIRE(ch3);
    BOOST_CHECK(ch3->name() == L"d_var");
    BOOST_CHECK(ch3->val() == L"30");
    BOOST_CHECK(ch3->type() == L"int");
    BOOST_CHECK(ch3->childs_size() == 0);
}


/// Tests displaying pointer to record with dynamic type and no address
BOOST_AUTO_TEST_CASE(ptr_record_dynamic_value_test_no_addr) {
    cm_builder b;
    auto cm_res = b
        .record("base1", "base1")
            .ivar("b1_var", b.bt_int())
        .end()

        .record("base2", "base2")
            .ivar("b2_var", b.bt_int())
        .end()

        .record("derived", "derived")
            .base(b.typeref("base1"))
            .base(b.typeref("base2"))
            .ivar("d_var", b.bt_int())
        .end()
    .build();

    auto rec = record_value{cm_res.rtype("derived")};
    rec["b1_var"] = 10;
    rec["b2_var"] = 20;
    rec["d_var"] = 30;

    auto base2 = rec.base(cm_res.rtype("base2"));
    auto ptr = base2.ptr();

    auto derived_ptr = cm.get_or_create_ptr_type(cm_res.rtype("derived"));
    vctx.add_dynamic_type(base2.addr(), vctx.get_or_create_type(derived_ptr), rec.addr());

    ctx.set_show_ptr_addr(false);
    auto res = ctx.format(ptr);
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"{ d_var=30 }");
    BOOST_CHECK(res->type() == L"derived*");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto base1_res = res->child_at(0);
    BOOST_REQUIRE(base1_res);
    BOOST_CHECK(base1_res->name() == L"base1");
    BOOST_CHECK(base1_res->val() == L"{ b1_var=10 }");
    BOOST_CHECK(base1_res->type() == L"base1");
    BOOST_REQUIRE_EQUAL(base1_res->childs_size(), 1);

    auto ch1 = base1_res->child_at(0);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"b1_var");
    BOOST_CHECK(ch1->val() == L"10");
    BOOST_CHECK(ch1->type() == L"int");
    BOOST_CHECK(ch1->childs_size() == 0);

    auto base2_res = res->child_at(1);
    BOOST_REQUIRE(base2_res);
    BOOST_CHECK(base2_res->name() == L"base2");
    BOOST_CHECK(base2_res->val() == L"{ b2_var=20 }");
    BOOST_CHECK(base2_res->type() == L"base2");
    BOOST_REQUIRE_EQUAL(base2_res->childs_size(), 1);

    auto ch2 = base2_res->child_at(0);
    BOOST_REQUIRE(ch2);
    BOOST_CHECK(ch2->name() == L"b2_var");
    BOOST_CHECK(ch2->val() == L"20");
    BOOST_CHECK(ch2->type() == L"int");
    BOOST_CHECK(ch2->childs_size() == 0);

    auto ch3 = res->child_at(2);
    BOOST_REQUIRE(ch3);
    BOOST_CHECK(ch3->name() == L"d_var");
    BOOST_CHECK(ch3->val() == L"30");
    BOOST_CHECK(ch3->type() == L"int");
    BOOST_CHECK(ch3->childs_size() == 0);
}


/// Tests displaying bad pointer to record
BOOST_AUTO_TEST_CASE(bad_ptr_record_value_test) {
    auto rtype = cm.create_named_record("my_rec");
    auto fld = rtype->create_field("fld", cm.bt_int());
    dbg.make_def_rec_layout(rtype);

    auto rec = error_value{rtype};

    auto res = ctx.format(rec.ptr());
    BOOST_REQUIRE(res);
    auto addr = addr_str(rec.addr());
    auto start_idx = addr.size() + 7;
    BOOST_CHECK(res->val().substr(0, start_idx) == addr + L" { fld=");
    BOOST_CHECK(is_error_msg(res->val().substr(start_idx, res->val().size() - start_idx - 2)));
    BOOST_CHECK(res->type() == L"my_rec*");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ch = res->child_at(0);
    BOOST_REQUIRE(ch);
    BOOST_CHECK(ch->name() == L"fld");
    BOOST_CHECK(is_error_msg(ch->val()));
    BOOST_CHECK(ch->type() == L"int");
    BOOST_CHECK(ch->childs_size() == 0);
}


/// Tests displaying pointer to function
BOOST_AUTO_TEST_CASE(fptr_test) {
    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);
    dbg.add_object(func, 0xAA, 100);

    pointer_value fptr{fptr_type, 0xAA};
    auto res = ctx.format(fptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x00000000000000AA &my_func");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests displaying pointer to function with ptr addresses disabled
BOOST_AUTO_TEST_CASE(fptr_test_no_addr) {
    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);
    dbg.add_object(func, 0xAA, 100);

    pointer_value fptr{fptr_type, 0xAA};
    ctx.set_show_ptr_addr(false);
    auto res = ctx.format(fptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"&my_func");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting pointer to function with offset
BOOST_AUTO_TEST_CASE(fptr_test_offset) {
    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);
    dbg.add_object(func, 0xA0, 100);

    pointer_value fptr{fptr_type, 0xAA};
    auto res = ctx.format(fptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x00000000000000AA &my_func + 10");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting pointer to function with offset in hex mode
BOOST_AUTO_TEST_CASE(fptr_test_offset_hex) {
    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);
    dbg.add_object(func, 0xA0, 100);

    pointer_value fptr{fptr_type, 0xAA};
    ctx.set_hex(true);
    auto res = ctx.format(fptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x00000000000000AA &my_func + 0x000000000000000A");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests displaying pointer to symbol
BOOST_AUTO_TEST_CASE(sym_test) {
    int_value ival{10};
    dbg.add_sym(ival.addr(), "my_sym", 8);
    auto res = ctx.format(ival.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(ival.addr()) + L" &my_sym 10");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->val() == L"10");
}


/// Tests formatting pointer to symbol with offset
BOOST_AUTO_TEST_CASE(sym_test_offset) {
    int_value ival{12};
    dbg.add_sym(ival.addr() - 10, "my_sym", 20);
    auto res = ctx.format(ival.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(ival.addr())+ L" &my_sym + 10 12");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->val() == L"12");
}


/// Tests formatting pointer to symbol with offset in hex mode
BOOST_AUTO_TEST_CASE(sym_test_offset_hex) {
    int_value ival{12};
    dbg.add_sym(ival.addr() - 10, "my_sym", 20);
    ctx.set_hex(true);
    auto res = ctx.format(ival.ptr());

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(ival.addr()) + L" &my_sym + 0x000000000000000A 0x0000000C");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->val() == L"0x0000000C");
}


/// Tests displaying pointer to null terminated wide string
BOOST_AUTO_TEST_CASE(pointer_null_term_wstring_test) {

    array_value str{cm.bt_wchar_t(), 6};
    const wchar_t * wstr = L"фыв\nж";

    for (size_t i = 0; i < 6; ++i) {
        str[i] = wstr[i];
    }

    auto ch_ptr = cm.get_or_create_ptr_type(cm.bt_wchar_t());
    auto res = ctx.format(str.ptr().cast(ch_ptr));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(str.addr()) + L" \"фыв\\nж\"");
    BOOST_CHECK(res->type() == L"wchar_t*");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"'ф' (1092)");
    BOOST_CHECK(child->type() == L"wchar_t");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests displaying pointer to null terminated unicode string
BOOST_AUTO_TEST_CASE(null_term_unicode_string) {

    const char * cstr = "фыв";
    size_t str_sz = strlen(cstr);
    array_value str{cm.bt_char(), str_sz + 1};

    for (size_t i = 0; i <= str_sz; ++i) {
        str[i] = cstr[i];
    }

    auto ch_ptr = cm.get_or_create_ptr_type(cm.bt_char());
    auto res = ctx.format(str.ptr().cast(ch_ptr));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(str.addr()) + L" \"фыв\"");
    BOOST_CHECK(res->type() == L"char*");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"'\\xD1' (-47)");
    BOOST_CHECK(child->type() == L"char");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests displaying pointer to null terminated unicode string with bad characters
BOOST_AUTO_TEST_CASE(null_term_unicode_bad_string) {
    const char * cstr = "фыв\x9Cy";
    size_t str_sz = strlen(cstr);

    array_value str{cm.bt_char(), str_sz + 1};

    for (size_t i = 0; i <= str_sz; ++i) {
        str[i] = cstr[i];
    }

    auto ch_ptr = cm.get_or_create_ptr_type(cm.bt_char());
    auto res = ctx.format(str.ptr().cast(ch_ptr));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(str.addr()) + L" \"фыв\\x9Cy\"");
    BOOST_CHECK(res->type() == L"char*");
    BOOST_CHECK(res->childs_size() == 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"");
    BOOST_CHECK(child->val() == L"'\\xD1' (-47)");
    BOOST_CHECK(child->type() == L"char");
    BOOST_CHECK(child->childs_size() == 0);
}


/// Tests formatting member pointer
BOOST_AUTO_TEST_CASE(mem_ptr) {
    cm_builder b;
    auto cmres = b
        .record("foo", "foo")
            .ivar("x", b.bt_int())
            .ivar("y", b.bt_int(), cm::access_level::public_, 0, {b.source("test.cpp"), 20})
        .end()
    .build();

    auto foo_type = cmres.rtype("foo");
    auto foo_mem_ptr_type = cm.get_or_create_mem_ptr_type(foo_type, cm.bt_int());
    long_value offset{4};

    auto res = ctx.format(offset.cast(foo_mem_ptr_type));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"&foo::y");
    BOOST_CHECK(res->type() == L"int foo::*");
    BOOST_CHECK_EQUAL(res->childs_size(), 0);
    BOOST_CHECK_EQUAL(res->val_pos().file(), "test.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 20);
}


BOOST_AUTO_TEST_SUITE_END()


}
