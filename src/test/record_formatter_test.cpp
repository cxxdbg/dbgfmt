
/// \file record_formatter_test.cpp
/// Contains unit tests for record formatter.

#include <boost/test/unit_test.hpp>
#include "test2.hpp"
#include "dbgfmt/source_position.hpp"
#include "stl/stdcpp/stdcpp_vector.hpp"


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(record_formatter_test, fmt_test_fixture)


/// Tests formatting record value
BOOST_AUTO_TEST_CASE(record_value_test) {
    source_position dpos = {"my_test.cpp", 50};

    // creating type for the first base
    auto vbase_type = cm.create_named_record("my_vbase");
    vbase_type->create_field("vbase_fld", cm.bt_int());
    vbase_type->set_loc({cm.source("my_test2.cpp"), 30});

    // creating type for the second base
    auto base_type = cm.create_named_record("my_base");
    base_type->create_field("base_fld", cm.bt_int());

    // creating record type
    auto rtype = cm.create_named_record("my_rec");
    rtype->set_loc({cm.source("my_test.cpp"), 50});
    rtype->add_base(vbase_type);
    rtype->add_base(base_type);
    auto fld = rtype->create_field("fld", cm.bt_int());
    fld->set_loc({cm.source("my_test.cpp"), 25});

    // creating debug info for all record types
    dbg.make_def_rec_layout(vbase_type);
    dbg.make_def_rec_layout(base_type);
    dbg.make_def_rec_layout(rtype);

    // creating record value
    auto rec = make_val(rtype);
    rec.base_at(0)["vbase_fld"] << 10;
    rec.base_at(1)["base_fld"] << 20;
    rec["fld"] << 30;

    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->type() == L"my_rec");
    BOOST_CHECK(res->val() == L"{ fld=30 }");

    BOOST_CHECK(res->type_pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(res->type_pos().line(), 50);

    BOOST_REQUIRE(res->childs_size() == 3);

    auto child1 = res->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"my_vbase");
    BOOST_CHECK(child1->type() == L"my_vbase");
    BOOST_CHECK(child1->val() == L"{ vbase_fld=10 }");
    BOOST_CHECK(child1->type_pos().file() == "my_test2.cpp");
    BOOST_CHECK_EQUAL(child1->type_pos().line(), 30);

    BOOST_REQUIRE(child1->childs_size() == 1);
    auto child1_child = child1->child_at(0);
    BOOST_REQUIRE(child1_child);
    BOOST_CHECK(child1_child->name() == L"vbase_fld");
    BOOST_CHECK(child1_child->type() == L"int");
    BOOST_CHECK(child1_child->val() == L"10");

    auto child2 = res->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"my_base");
    BOOST_CHECK(child2->type() == L"my_base");
    BOOST_CHECK(child2->val() == L"{ base_fld=20 }");
    BOOST_REQUIRE(child2->childs_size() == 1);
    auto child2_child = child2->child_at(0);
    BOOST_REQUIRE(child2_child);
    BOOST_CHECK(child2_child->name() == L"base_fld");
    BOOST_CHECK(child2_child->type() == L"int");
    BOOST_CHECK(child2_child->val() == L"20");

    auto child3 = res->child_at(2);
    BOOST_REQUIRE(child3);
    BOOST_CHECK(child3->name() == L"fld");
    BOOST_CHECK(child3->type() == L"int");
    BOOST_CHECK(child3->val() == L"30");
    BOOST_CHECK(child3->childs_size() == 0);
    BOOST_CHECK(child3->pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(child3->pos().line(), 25);
}


/// Tests formatting record value with address
BOOST_AUTO_TEST_CASE(record_value_test_addr) {
    source_position dpos = {"my_test.cpp", 50};

    // creating type for the first base
    auto vbase_type = cm.create_named_record("my_vbase");
    vbase_type->create_field("vbase_fld", cm.bt_int());
    vbase_type->set_loc({cm.source("my_test2.cpp"), 30});

    // creating type for the second base
    auto base_type = cm.create_named_record("my_base");
    base_type->create_field("base_fld", cm.bt_int());

    // creating record type
    auto rtype = cm.create_named_record("my_rec");
    rtype->set_loc({cm.source("my_test.cpp"), 50});
    rtype->add_base(vbase_type);
    rtype->add_base(base_type);
    auto fld = rtype->create_field("fld", cm.bt_int());
    fld->set_loc({cm.source("my_test.cpp"), 25});

    // creating debug info for all record types
    dbg.make_def_rec_layout(vbase_type);
    dbg.make_def_rec_layout(base_type);
    dbg.make_def_rec_layout(rtype);

    // creating record value
    auto rec = make_val(rtype);
    rec.base_at(0)["vbase_fld"] << 10;
    rec.base_at(1)["base_fld"] << 20;
    rec["fld"] << 30;

    auto opts = ctx.fmt_opts();
    opts.set_show_rec_addr(true);
    ctx.set_fmt_opts(opts);
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->type() == L"my_rec");
    BOOST_CHECK(res->val() == addr_str(rec.address()) + L" { fld=30 }");

    BOOST_CHECK(res->type_pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(res->type_pos().line(), 50);

    BOOST_REQUIRE(res->childs_size() == 3);

    auto child1 = res->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"my_vbase");
    BOOST_CHECK(child1->type() == L"my_vbase");
    BOOST_CHECK(child1->val() == addr_str(rec.base_at(0).address()) + L" { vbase_fld=10 }");
    BOOST_CHECK(child1->type_pos().file() == "my_test2.cpp");
    BOOST_CHECK_EQUAL(child1->type_pos().line(), 30);

    BOOST_REQUIRE(child1->childs_size() == 1);
    auto child1_child = child1->child_at(0);
    BOOST_REQUIRE(child1_child);
    BOOST_CHECK(child1_child->name() == L"vbase_fld");
    BOOST_CHECK(child1_child->type() == L"int");
    BOOST_CHECK(child1_child->val() == L"10");

    auto child2 = res->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"my_base");
    BOOST_CHECK(child2->type() == L"my_base");
    BOOST_CHECK(child2->val() == addr_str(rec.base_at(1).address()) + L" { base_fld=20 }");
    BOOST_REQUIRE(child2->childs_size() == 1);
    auto child2_child = child2->child_at(0);
    BOOST_REQUIRE(child2_child);
    BOOST_CHECK(child2_child->name() == L"base_fld");
    BOOST_CHECK(child2_child->type() == L"int");
    BOOST_CHECK(child2_child->val() == L"20");

    auto child3 = res->child_at(2);
    BOOST_REQUIRE(child3);
    BOOST_CHECK(child3->name() == L"fld");
    BOOST_CHECK(child3->type() == L"int");
    BOOST_CHECK(child3->val() == L"30");
    BOOST_CHECK(child3->childs_size() == 0);
    BOOST_CHECK(child3->pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(child3->pos().line(), 25);
}


/// Tests formatting record value in hex mode
BOOST_AUTO_TEST_CASE(record_value_test_hex) {
    source_position dpos = {"my_test.cpp", 50};

    // creating type for the first base
    auto vbase_type = cm.create_named_record("my_vbase");
    vbase_type->create_field("vbase_fld", cm.bt_int());
    vbase_type->set_loc({cm.source("my_test2.cpp"), 30});

    // creating type for the second base
    auto base_type = cm.create_named_record("my_base");
    base_type->create_field("base_fld", cm.bt_int());

    // creating record type
    auto rtype = cm.create_named_record("my_rec");
    rtype->set_loc({cm.source("my_test.cpp"), 50});
    rtype->add_base(vbase_type);
    rtype->add_base(base_type);
    auto fld = rtype->create_field("fld", cm.bt_int());
    fld->set_loc({cm.source("my_test.cpp"), 25});

    // creating debug info for all record types
    dbg.make_def_rec_layout(vbase_type);
    dbg.make_def_rec_layout(base_type);
    dbg.make_def_rec_layout(rtype);

    // creating record value
    auto rec = make_val(rtype);
    rec.base_at(0)["vbase_fld"] << 10;
    rec.base_at(1)["base_fld"] << 20;
    rec["fld"] << 30;

    ctx.set_hex(true);
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->type() == L"my_rec");
    BOOST_CHECK(res->val() == L"{ fld=0x0000001E }");

    BOOST_CHECK(res->type_pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(res->type_pos().line(), 50);

    BOOST_REQUIRE(res->childs_size() == 3);

    auto child1 = res->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"my_vbase");
    BOOST_CHECK(child1->type() == L"my_vbase");
    BOOST_CHECK(child1->val() == L"{ vbase_fld=0x0000000A }");
    BOOST_CHECK(child1->type_pos().file() == "my_test2.cpp");
    BOOST_CHECK_EQUAL(child1->type_pos().line(), 30);

    BOOST_REQUIRE(child1->childs_size() == 1);
    auto child1_child = child1->child_at(0);
    BOOST_REQUIRE(child1_child);
    BOOST_CHECK(child1_child->name() == L"vbase_fld");
    BOOST_CHECK(child1_child->type() == L"int");
    BOOST_CHECK(child1_child->val() == L"0x0000000A");

    auto child2 = res->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"my_base");
    BOOST_CHECK(child2->type() == L"my_base");
    BOOST_CHECK(child2->val() == L"{ base_fld=0x00000014 }");
    BOOST_REQUIRE(child2->childs_size() == 1);
    auto child2_child = child2->child_at(0);
    BOOST_REQUIRE(child2_child);
    BOOST_CHECK(child2_child->name() == L"base_fld");
    BOOST_CHECK(child2_child->type() == L"int");
    BOOST_CHECK(child2_child->val() == L"0x00000014");

    auto child3 = res->child_at(2);
    BOOST_REQUIRE(child3);
    BOOST_CHECK(child3->name() == L"fld");
    BOOST_CHECK(child3->type() == L"int");
    BOOST_CHECK(child3->val() == L"0x0000001E");
    BOOST_CHECK(child3->childs_size() == 0);
    BOOST_CHECK(child3->pos().file() == "my_test.cpp");
    BOOST_CHECK_EQUAL(child3->pos().line(), 25);
}


/// Tests formatting bitfield
BOOST_AUTO_TEST_CASE(bitfield_test) {
    cm_builder b;
    auto cmres = b
        .record("myrec", "myrec")
            .ivar("fld1", b.bt_unsigned_int(), cm::access_level::public_, 6)
            .ivar("fld2", b.bt_unsigned_int(), cm::access_level::public_, 12)
        .end()
    .build();

    auto arr_type = cm.get_or_create_arr_type(cm.bt_unsigned_char(), 8);
    auto arr = make_val(arr_type);
    arr[0] << (unsigned char)0b11101010U;
    arr[1] << (unsigned char)0b00101100U;
    arr[2] << (unsigned char)0b00100010U;
    arr[3] << (unsigned char)0;
    arr[4] << (unsigned char)0;
    arr[5] << (unsigned char)0;
    arr[6] << (unsigned char)0;
    arr[7] << (unsigned char)0;

    auto rec = arr.cast(vctx.get_or_create_type(cmres.rtype("myrec")));
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"myrec");
    BOOST_CHECK(res->val() == L"{ fld1=42 fld2=2227 }");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 2);

    auto fld1 = res->child_at(0);
    BOOST_CHECK(fld1->type() == L"unsigned int");
    BOOST_CHECK(fld1->val() == L"42");
    BOOST_CHECK_EQUAL(fld1->childs_size(), 0);

    auto fld2 = res->child_at(1);
    BOOST_CHECK(fld2->type() == L"unsigned int");
    BOOST_CHECK(fld2->val() == L"2227");
    BOOST_CHECK_EQUAL(fld2->childs_size(), 0);
}


/// Tests formatting bitfield in hex mode
BOOST_AUTO_TEST_CASE(bitfield_test_hex) {
    cm_builder b;
    auto cmres = b
        .record("myrec", "myrec")
            .ivar("fld1", b.bt_unsigned_int(), cm::access_level::public_, 6)
            .ivar("fld2", b.bt_unsigned_int(), cm::access_level::public_, 12)
        .end()
    .build();

    auto arr_type = cm.get_or_create_arr_type(cm.bt_unsigned_char(), 8);
    auto arr = make_val(arr_type);
    arr[0] << (unsigned char)0b11101010U;
    arr[1] << (unsigned char)0b00101100U;
    arr[2] << (unsigned char)0b00100010U;
    arr[3] << (unsigned char)0;
    arr[4] << (unsigned char)0;
    arr[5] << (unsigned char)0;
    arr[6] << (unsigned char)0;
    arr[7] << (unsigned char)0;

    auto rec = arr.cast(vctx.get_or_create_type(cmres.rtype("myrec")));
    ctx.set_hex(true);
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"myrec");
    BOOST_CHECK(res->val() == L"{ fld1=0x0000002A fld2=0x000008B3 }");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 2);

    auto fld1 = res->child_at(0);
    BOOST_CHECK(fld1->type() == L"unsigned int");
    BOOST_CHECK(fld1->val() == L"0x0000002A");
    BOOST_CHECK_EQUAL(fld1->childs_size(), 0);

    auto fld2 = res->child_at(1);
    BOOST_CHECK(fld2->type() == L"unsigned int");
    BOOST_CHECK(fld2->val() == L"0x000008B3");
    BOOST_CHECK_EQUAL(fld2->childs_size(), 0);
}


/// Tests formatting bitfield with read error
BOOST_AUTO_TEST_CASE(bitfield_test_error) {
    cm_builder b;
    auto cmres = b
        .record("myrec", "myrec")
            .ivar("fld1", b.bt_unsigned_int(), cm::access_level::public_, 6)
            .ivar("fld2", b.bt_unsigned_int(), cm::access_level::public_, 12)
        .end()
    .build();

    auto atype = cm.get_or_create_arr_type(cm.bt_unsigned_char(), 8);
    auto err_array = make_err(atype);
    auto rec = err_array.cast(vctx.get_or_create_type(cmres.rtype("myrec")));
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"myrec");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 2);

    auto fld1 = res->child_at(0);
    BOOST_CHECK(is_error_msg(fld1->val()));
    BOOST_CHECK_EQUAL(fld1->childs_size(), 0);

    auto fld2 = res->child_at(1);
    BOOST_CHECK(is_error_msg(fld2->val()));
    BOOST_CHECK_EQUAL(fld2->childs_size(), 0);
}


/// Tests displaying record with infinite recursion
BOOST_AUTO_TEST_CASE(record_value_test_infinite_ptr) {
    auto rtype = cm.create_named_record("my_rec");
    auto ptr_type = cm.get_or_create_ptr_type(rtype);
    auto fld = rtype->create_field("fld", ptr_type);
    dbg.make_def_rec_layout(rtype);

    auto rec = make_val(rtype);
    rec["fld"] << rec.ptr();

    // building result string
    std::wostringstream str_str;
    for (int i = 0; i < 10; ++i) {
        str_str << L"{ fld=" + addr_str(rec.address()) + L" ";
    }

    str_str << L"{ ... }";

    for (int i = 0; i < 10; ++i) {
        str_str << L" }";
    }

    const auto & str = str_str.str();

    auto res = ctx.format(rec);
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == str);
    BOOST_CHECK(res->type() == L"my_rec");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ch = res->child_at(0);
    BOOST_REQUIRE(ch);
    BOOST_CHECK(ch->name() == L"fld");
    BOOST_CHECK(ch->type() == L"my_rec*");
    BOOST_CHECK(ch->childs_size() == 1);
}


/// Tests using custom formatter for formatting base classes of record
BOOST_AUTO_TEST_CASE(base_custom_formatter) {
    auto vec_cm = stdcpp_vector_cm(cm.bt_int());
    cm_builder b;
    auto cm_res = b
        .record("my_record", "my_record")
            .base(vec_cm.type("stdcpp_vector"))
        .end()
    .build();

    std::vector<value> values = {make_int(10), make_int(20), make_int(30)};
    auto vector_val = make_stdcpp_vector(cm.bt_int(), values);
    auto rec = make_val(cm_res.type("my_record"));
    rec.base_at(0) << vector_val;

    auto res = ctx.format(rec);
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"{  }");
    BOOST_CHECK(res->type() == L"my_record");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);
    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"std::vector<int>");
    BOOST_CHECK(child->val() == L"[size = 3, capacity = 3]");
    BOOST_CHECK_EQUAL(child->childs_size(), 5);
}


/// Tests formatting record as functor
BOOST_AUTO_TEST_CASE(record_as_functor) {
    auto rtype  = cm.create_named_record("my_rec");
    rtype->create_field("x", cm.bt_int());

    auto call_op = rtype->create_method("operator()");
    call_op->set_ret_type(cm.bt_void());
    call_op->add_param(cm.bt_int());
    call_op->add_param(cm.bt_float());
    call_op->set_loc({cm.source("my_rec.cpp"), 55});
    dbg.add_object(call_op, 0xA0, 100);

    dbg.make_def_rec_layout(rtype);

    auto rec = make_val(rtype);
    rec["x"] << 10;

    auto res = ctx.format(rec, ctx.fmt_opts().with_rec_as_functor(true));
    BOOST_REQUIRE(res);

    std::wstring lambda_name = L"0x00000000000000A0 &my_rec::operator()";

    BOOST_CHECK(res->val() == lambda_name);
    BOOST_CHECK_EQUAL(res->val_pos().file(), "my_rec.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 55);

    BOOST_REQUIRE(res->childs_size() == 2);

    auto func = res->child_at(0);
    BOOST_REQUIRE(func);
    BOOST_CHECK(func->name() == L"[target]");
    BOOST_CHECK(func->val() == lambda_name);
    BOOST_CHECK(func->type() == L"void (*)(int, float)");
    BOOST_CHECK(func->childs_size() == 0);
    BOOST_CHECK_EQUAL(func->val_pos().file(), "my_rec.cpp");
    BOOST_CHECK_EQUAL(func->val_pos().line(), 55);

    auto x = res->child_at(1);
    BOOST_REQUIRE(x);
    BOOST_CHECK(x->name() == L"x");
    BOOST_CHECK(x->val() == L"10");
    BOOST_CHECK(x->type() == L"int");
}


BOOST_AUTO_TEST_SUITE_END()

}
