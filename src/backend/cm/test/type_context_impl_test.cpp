
/// \file type_context_impl.cpp
/// Contains unit tests for the type_context_impl class.

#include "../array_type_impl.hpp"
#include "../builtin_type_impl.hpp"
#include "../enum_type_impl.hpp"
#include "../function_type_impl.hpp"
#include "../mem_ptr_type_impl.hpp"
#include "../record_type_impl.hpp"
#include "../type_context_impl.hpp"
#include "../typedef_type_impl.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::backend::cm::test {


struct type_context_impl_test_fixture {
    ::cm::code_model cm;
    ::cm::debug_info dbg{cm};
    type_context_impl ctx{cm, dbg};
};


BOOST_FIXTURE_TEST_SUITE(type_context_impl_test, type_context_impl_test_fixture)


/// Tests creating builtin type
BOOST_AUTO_TEST_CASE(create_builtin) {
    auto int_type = cm.bt_int();
    auto fmtcm_type = ctx.get_or_create_type(int_type);
    auto fmtcm_int_type = dynamic_cast<builtin_type_impl*>(fmtcm_type);
    BOOST_REQUIRE(fmtcm_int_type->kind() == builtin_type_impl::kind_t::int_);
}


/// Tests creating typedef type in FMTCM
BOOST_AUTO_TEST_CASE(create_typedef) {
    auto td = cm.create_typedef("my_int", cm.bt_int());
    auto fmtcm_type = ctx.get_or_create_type(td);
    auto fmtcm_td = dynamic_cast<typedef_type_impl*>(fmtcm_type);

    BOOST_REQUIRE(fmtcm_td);
    BOOST_CHECK_EQUAL(fmtcm_td->name(), "my_int");
    BOOST_CHECK(fmtcm_td->base() == ctx.get_or_create_type(cm.bt_int()));
}


/// Tests creating enum type in FMTCM
BOOST_AUTO_TEST_CASE(create_enum) {
    auto en = cm.create_enum("my_enum", cm.bt_int());
    en->items().push_back({"my_val1", 100});
    en->items().push_back({"my_val2", 200});
    auto fmtcm_type = ctx.get_or_create_type(en);
    auto fmtcm_en = dynamic_cast<enum_type_impl*>(fmtcm_type);

    BOOST_REQUIRE(fmtcm_en);
    BOOST_CHECK_EQUAL(fmtcm_en->name(), "my_enum");
    BOOST_REQUIRE_EQUAL(fmtcm_en->items_size(), 2);
    BOOST_CHECK_EQUAL(fmtcm_en->item_name(0), "my_val1");
    BOOST_CHECK_EQUAL(fmtcm_en->item_value(0), 100);
    BOOST_CHECK_EQUAL(fmtcm_en->item_name(1), "my_val2");
    BOOST_CHECK_EQUAL(fmtcm_en->item_value(1), 200);
}


/// Tests creating array type
BOOST_AUTO_TEST_CASE(create_array) {
    auto int_type = cm.bt_int();
    auto arr_type = cm.get_or_create_arr_type(int_type, 10);
    auto fmtcm_type = ctx.get_or_create_type(arr_type);
    auto fmtcm_arr_type = dynamic_cast<array_type_impl*>(fmtcm_type);

    BOOST_REQUIRE(fmtcm_arr_type);
    BOOST_CHECK_EQUAL(fmtcm_arr_type->array_size(), 10);
    BOOST_CHECK(fmtcm_arr_type->base() == ctx.get_or_create_type(cm.bt_int()));
}


/// Tests creating record type
BOOST_AUTO_TEST_CASE(create_record) {
    // creating type for base 1
    auto base1_type = cm.create_named_record("base1");
    base1_type->create_field("base1_fld", cm.bt_int(), ::cm::access_level::private_);

    // creating type for base 2
    auto base2_type = cm.create_named_record("base2");
    base2_type->create_field("base2_fld_1", cm.bt_int(), ::cm::access_level::private_);
    base2_type->create_field("base2_fld_2", cm.bt_float(), ::cm::access_level::private_);

    // creating type for record
    auto rec_type = cm.create_named_record("my_rec");
    rec_type->add_base(base1_type);
    rec_type->add_base(base2_type);
    rec_type->create_field("fld_1", cm.bt_int(), ::cm::access_level::private_);
    rec_type->create_field("fld_2", cm.bt_double(), ::cm::access_level::private_);

    // creating FMTCM type
    auto res_untyped = ctx.get_or_create_type(rec_type);
    auto res = dynamic_cast<const dbgfmt::backend::record_type_impl*>(res_untyped);

    BOOST_REQUIRE(res);
    BOOST_REQUIRE_EQUAL(res->bases_size(), 2);

    auto base1 = res->base_type_at(0);
    BOOST_REQUIRE(base1);
    BOOST_CHECK(base1 == dynamic_cast<const dbgfmt::backend::record_type_impl*>(ctx.get_or_create_type(base1_type)));
    BOOST_CHECK_EQUAL(base1->template_name(), "base1");
    BOOST_CHECK_EQUAL(base1->template_param_names_size(), 0);
    BOOST_CHECK_EQUAL(base1->bases_size(), 0);
    BOOST_REQUIRE_EQUAL(base1->fields_size(), 1);
    BOOST_CHECK_EQUAL(base1->field_name_at(0), "base1_fld");
    BOOST_CHECK(!base1->field_def_pos_at(0));
    BOOST_CHECK(base1->field_type_at(0) == ctx.bt_int());

    auto base2 = res->base_type_at(1);
    BOOST_REQUIRE(base2);
    BOOST_CHECK(base2 == dynamic_cast<const dbgfmt::backend::record_type_impl*>(ctx.get_or_create_type(base2_type)));
    BOOST_CHECK_EQUAL(base2->template_name(), "base2");
    BOOST_CHECK_EQUAL(base2->template_param_names_size(), 0);
    BOOST_CHECK_EQUAL(base2->bases_size(), 0);
    BOOST_REQUIRE_EQUAL(base2->fields_size(), 2);
    BOOST_CHECK_EQUAL(base2->field_name_at(0), "base2_fld_1");
    BOOST_CHECK(!base2->field_def_pos_at(0));
    BOOST_CHECK(base2->field_type_at(0) == ctx.bt_int());
    BOOST_CHECK_EQUAL(base2->field_name_at(1), "base2_fld_2");
    BOOST_CHECK(!base2->field_def_pos_at(1));
    BOOST_CHECK(base2->field_type_at(1) == ctx.bt_float());

    BOOST_CHECK_EQUAL(res->template_name(), "my_rec");
    BOOST_CHECK_EQUAL(res->template_param_names_size(), 0);
    BOOST_REQUIRE_EQUAL(res->fields_size(), 2);
    BOOST_CHECK_EQUAL(res->field_name_at(0), "fld_1");
    BOOST_CHECK(!res->field_def_pos_at(0));
    BOOST_CHECK(res->field_type_at(0) == ctx.bt_int());
    BOOST_CHECK_EQUAL(res->field_name_at(1), "fld_2");
    BOOST_CHECK(!res->field_def_pos_at(1));
    BOOST_CHECK(res->field_type_at(1) == ctx.bt_double());
}


/// Tests creating function type impl
BOOST_AUTO_TEST_CASE(create_function_type) {
    auto ftype = cm.get_or_create_func_type(cm.bt_int(), cm.bt_float(), cm.bt_int());
    auto fmtcm_type = ctx.get_or_create_type(ftype)->cast<function_type_impl>();

    BOOST_REQUIRE(fmtcm_type);
    BOOST_CHECK(fmtcm_type->ret_type() == ctx.get_or_create_type(cm.bt_int()));
    BOOST_REQUIRE_EQUAL(fmtcm_type->params_size(), 2);
    BOOST_CHECK(fmtcm_type->param_at(0) == ctx.get_or_create_type(cm.bt_float()));
    BOOST_CHECK(fmtcm_type->param_at(1) == ctx.get_or_create_type(cm.bt_int()));
}


/// Tests creating member pointer type impl
BOOST_AUTO_TEST_CASE(create_mem_ptr_type) {
    auto rtype = cm.create_named_record("rec");
    auto mtype = cm.get_or_create_mem_ptr_type(rtype, cm.bt_int());

    auto res = ctx.get_or_create_type(mtype);
    auto mptr_res = dynamic_cast<dbgfmt::backend::mem_ptr_type_impl*>(res);

    BOOST_REQUIRE(mptr_res);
    BOOST_CHECK(static_cast<const dbgfmt::backend::type_impl*>(mptr_res->obj_type()) ==
                ctx.get_or_create_type(rtype));
    BOOST_CHECK(mptr_res->mem_type() == ctx.get_or_create_type(cm.bt_int()));
}


BOOST_AUTO_TEST_SUITE_END()


}
