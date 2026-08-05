
/// \file stdcpp_bind_result_formatter_test.cpp
/// Contains unit tests for libstdc++ bind result formatters

#include "libcxx/libcxx_bind.hpp"
#include "stdcpp/stdcpp_tuple.hpp"
#include "stdcpp/stdcpp_bind.hpp"
#include "../test2.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


namespace {
    enum bind_kind {
        stdcpp,
        libcxx
    };
}


BOOST_FIXTURE_TEST_SUITE(stdcpp_bind_result_formatter_test, fmt_test_fixture)


/// Creates vector of bound arguments for test
static std::vector<value> make_test_args(bind_kind knd) {
    auto first_type = knd == bind_kind::stdcpp ?
                             get_stdcpp_placeholder_type(cm::value{1}) :
                             get_libcxx_placeholder_type(cm::value{1});
    auto first = make_val(first_type);
    auto second = make_int(10);
    return {first, second};
}


/// Creates result of std::bind with specified functor and bound arguments
static value make_bind(bind_kind knd, const value & functor, const std::vector<value> & bound_args) {
    if (knd == bind_kind::stdcpp) {
        return make_stdcpp_bind(functor, bound_args);
    } else {
        return make_libcxx_bind(functor, bound_args);
    }
}


/// Tests formatting bind result with custom user defined functor
void test_custom(context & ctx, bind_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    // creating target function type
    auto ftype = cm.get_or_create_func_type(cm.bt_void(), cm.bt_int(), cm.bt_int());

    // creating functor type
    auto functor_type = cm.create_named_record("my_functor");
    functor_type->set_loc({cm.source("test.hpp"), 20});
    functor_type->create_field("memvar", cm.bt_int());

    auto call_op = functor_type->create_method("operator()");
    call_op->set_ret_type(cm.bt_void());
    call_op->add_param(cm.bt_int());
    call_op->add_param(cm.bt_float());
    call_op->set_loc({cm.source("test.hpp"), 22});

    dbg.add_object(call_op, 0xA0, 100);
    dbg.make_def_rec_layout(functor_type);

    // creating functor value
    auto my_functor = make_val(functor_type);
    my_functor["memvar"] << 20;

    // creating std::_Bind value
    auto bind_res = make_bind(knd, my_functor, make_test_args(knd));

    auto res = ctx.format(bind_res);

    auto ph_name = knd == bind_kind::stdcpp ? L"std::_Placeholder<1>" : L"std::__1::placeholders::__ph<1>";

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"std::bind(&my_functor::operator(), _1, 10)");
    if (knd == bind_kind::stdcpp) {
        BOOST_CHECK(res->type() == L"std::_Bind<my_functor(std::_Placeholder<1>, int)>");
    } else {
        BOOST_CHECK(res->type() == L"std::__1::__bind<my_functor, std::__1::placeholders::__ph<1>, int>");
    }

    BOOST_CHECK(res->val_pos().file() == "test.hpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 22);

    BOOST_REQUIRE(res->childs_size() == 2);

    auto target = res->child_at(0);
    BOOST_REQUIRE(target);
    BOOST_CHECK(target->name() == L"[target]");
    BOOST_CHECK(target->val() == L"{ memvar=20 }");
    BOOST_CHECK(target->type() == L"my_functor");

    BOOST_REQUIRE_EQUAL(target->childs_size(), 1);
    auto target_mem = target->child_at(0);
    BOOST_REQUIRE(target_mem);
    BOOST_CHECK(target_mem->name() == L"memvar");
    BOOST_CHECK(target_mem->val() == L"20");
    BOOST_CHECK(target_mem->type() == L"int");

    auto args = res->child_at(1);
    BOOST_REQUIRE(args);
    BOOST_CHECK(args->name() == L"[args]");
    BOOST_CHECK(args->val() == L"<_1, 10>");

    BOOST_REQUIRE_EQUAL(args->childs_size(), 2);

    auto child1 = args->child_at(0);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[0]");
    BOOST_CHECK(child1->val() == L"_1");
    BOOST_CHECK(child1->type() == ph_name);
    BOOST_CHECK_EQUAL(child1->childs_size(), 0);

    auto child2 = args->child_at(1);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[1]");
    BOOST_CHECK(child2->val() == L"10");
    BOOST_CHECK(child2->type() == L"int");
    BOOST_CHECK_EQUAL(child1->childs_size(), 0);
}

/// Tests formatting libstdc++ bind result with custom user defined functor
BOOST_AUTO_TEST_CASE(stdcpp_custom) {
    test_custom(ctx, bind_kind::stdcpp);
}

/// Tests formatting libc++ bind result with custom user defined functor
BOOST_AUTO_TEST_CASE(libcxx_custom) {
    test_custom(ctx, bind_kind::libcxx);
}


/// Tests formatting bind result with function pointer
void test_func_ptr(context & ctx, bind_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    // creating function
    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    func->add_param(cm.bt_int());
    func->add_param(cm.bt_int());
    func->set_loc({cm.source("my_file.cpp"), 45});
    dbg.add_object(func, 0xA2, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    // creating std::_Bind type
    auto bind_obj = make_bind(knd, make_ptr(fptr_type, 0xAA), make_test_args(knd));
    auto res = ctx.format(bind_obj);

    auto ph_name = knd == bind_kind::stdcpp ? L"std::_Placeholder<1>" : L"std::__1::placeholders::__ph<1>";

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"std::bind(&my_func + 8, _1, 10)");
    BOOST_CHECK(res->type() == val_type_name(bind_obj));
    BOOST_CHECK(res->val_pos().file() == "my_file.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 45);

    BOOST_REQUIRE(res->childs_size() == 2);

    auto target = res->child_at(0);
    BOOST_REQUIRE(target);
    BOOST_CHECK(target->name() == L"[target]");
    BOOST_CHECK(target->val() == L"0x00000000000000AA &my_func + 8");
    BOOST_CHECK(target->type() == L"void (*)(int, int)");

    BOOST_CHECK_EQUAL(target->childs_size(), 0);

    auto args = res->child_at(1);
    BOOST_REQUIRE(args);
    BOOST_CHECK(args->name() == L"[args]");
    BOOST_CHECK(args->val() == L"<_1, 10>");
}

/// Tests formatting libstdc++ bind result with function pointer
BOOST_AUTO_TEST_CASE(stdcpp_func_ptr) {
    test_func_ptr(ctx, bind_kind::stdcpp);
}

/// Tests formatting libc++ bind result with function pointer
BOOST_AUTO_TEST_CASE(libcxx_func_ptr) {
    test_func_ptr(ctx, bind_kind::libcxx);
}


/// Tests formatting bind result with old member function pointer
void test_old_mem_fun_ptr(context & ctx, bind_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    // creating record type
    auto rtype = cm.create_named_record("my_class");
    rtype->create_field("memvar", cm.bt_int());
    auto inst_func = rtype->create_method("mem_fun");
    inst_func->set_ret_type(cm.bt_void());
    inst_func->add_param(cm.bt_int());
    inst_func->add_param(cm.bt_int());
    dbg.make_def_rec_layout(rtype);
    dbg.add_object(inst_func, 0xAA, 100);

    // creating function type and pointer to function type
    auto ftype = cm.get_or_create_func_type(inst_func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    // creating record object
    auto robj = make_val(rtype);
    robj["memvar"] << 20;

    // creating member function type
    auto mem_fun_type = cm.create_named_record("(anonymous struct)");
    mem_fun_type->create_field("__pfn", fptr_type);
    mem_fun_type->create_field("__delta", cm.bt_long());
    dbg.make_def_rec_layout(mem_fun_type);

    // creating member function object
    auto mem_fun_obj = make_val(mem_fun_type);
    mem_fun_obj["__pfn"] << make_ptr(fptr_type, 0xAA);
    mem_fun_obj["__delta"] << make_long(0);

    // creating member function wrapper type
    auto std = cm.get_or_create_namespace("std");
    auto mem_func_wrap_type = std->create_named_record("_Mem_fn");
    mem_func_wrap_type->create_field("_M_pmf", mem_fun_type);
    dbg.make_def_rec_layout(mem_func_wrap_type);

    // creating member function wrapper object
    auto mem_func_wrap = make_val(mem_func_wrap_type);
    mem_func_wrap["_M_pmf"] << mem_fun_obj;

    // creating tuple type for bound args
    auto placeholder_type = knd == bind_kind::stdcpp ?
                                   get_stdcpp_placeholder_type(cm::value{1}) :
                                   get_libcxx_placeholder_type(cm::value{1});

    // creating std::_Bind type
    auto bind_obj = knd == bind_kind::stdcpp ?
                    make_stdcpp_bind(mem_func_wrap, {robj, make_val(placeholder_type), make_int(10)}) :
                    make_libcxx_bind(mem_fun_obj, {robj, make_val(placeholder_type), make_int(10)});

    auto res = ctx.format(bind_obj);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"std::bind(&my_class::mem_fun, { memvar=20 }, _1, 10)");
    BOOST_CHECK(res->type() == val_type_name(bind_obj));

    BOOST_REQUIRE(res->childs_size() == 2);

    auto target = res->child_at(0);
    BOOST_REQUIRE(target);
    BOOST_CHECK(target->name() == L"[target]");
    BOOST_CHECK(target->val() == L"0x00000000000000AA &my_class::mem_fun");
    BOOST_CHECK(target->type() == L"void (*)(int, int)");

    BOOST_CHECK_EQUAL(target->childs_size(), 0);

    auto args = res->child_at(1);
    BOOST_REQUIRE(args);
    BOOST_CHECK(args->name() == L"[args]");
    BOOST_CHECK(args->val() == L"<{ memvar=20 }, _1, 10>");
    BOOST_CHECK_EQUAL(args->childs_size(), 3);
}

/// Tests formatting libstdc++ bind result with old member function pointer
BOOST_AUTO_TEST_CASE(stdcpp_old_mem_fun_ptr) {
    test_old_mem_fun_ptr(ctx, bind_kind::stdcpp);
}

/// Tests formatting libc++ bind result with old member function pointer
BOOST_AUTO_TEST_CASE(libcxx_old_mem_fun_ptr) {
    test_old_mem_fun_ptr(ctx, bind_kind::libcxx);
}


/// Tests formatting bind result with member function pointer
void test_mem_fun_ptr(context & ctx, bind_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();
    auto & vctx = test_context::current_context().vctx();

    // creating record type
    auto rtype = cm.create_named_record("my_class");
    rtype->create_field("memvar", cm.bt_int());
    auto inst_func = rtype->create_method("mem_fun");
    inst_func->set_ret_type(cm.bt_void());
    inst_func->add_param(cm.bt_int());
    inst_func->add_param(cm.bt_int());
    dbg.make_def_rec_layout(rtype);
    dbg.add_object(inst_func, 0xAA, 100);

    // creating function type and pointer to function type
    auto ftype = cm.get_or_create_func_type(inst_func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    // creating record object
    auto robj = make_val(rtype);
    robj["memvar"] << 20;

    // creating pointer to member function type
    auto mem_fun_type = cm.get_or_create_mem_ptr_type(rtype, ftype);

    auto mem_fun_obj = make_val(mem_fun_type);
    auto void_ptr_type = vctx.get_or_create_type(cm.get_or_create_ptr_type(cm.bt_void()));
    mem_fun_obj.cast(void_ptr_type).write_uint64(0xAA);

    // creating member function wrapper type
    auto std = cm.get_or_create_namespace("std");
    auto mem_func_wrap_type = std->create_named_record("_Mem_fn");
    mem_func_wrap_type->create_field("_M_pmf", mem_fun_type);
    dbg.make_def_rec_layout(mem_func_wrap_type);

    // creating member function wrapper object
    auto mem_func_wrap = make_val(mem_func_wrap_type);
    mem_func_wrap["_M_pmf"] << mem_fun_obj;

    // creating tuple type for bound args
    auto placeholder_type = knd == bind_kind::stdcpp ?
                                   get_stdcpp_placeholder_type(cm::value{1}) :
                                   get_libcxx_placeholder_type(cm::value{1});

    // creating std::_Bind type
    auto bind_obj = knd == bind_kind::stdcpp ?
                    make_stdcpp_bind(mem_func_wrap, {robj, make_val(placeholder_type), make_int(10)}) :
                    make_libcxx_bind(mem_fun_obj, {robj, make_val(placeholder_type), make_int(10)});

    auto res = ctx.format(bind_obj);

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"std::bind(&my_class::mem_fun, { memvar=20 }, _1, 10)");
    BOOST_CHECK(res->type() == val_type_name(bind_obj));

    BOOST_REQUIRE(res->childs_size() == 2);

    auto target = res->child_at(0);
    BOOST_REQUIRE(target);
    BOOST_CHECK(target->name() == L"[target]");
    BOOST_CHECK(target->val() == L"0x00000000000000AA &my_class::mem_fun");
    BOOST_CHECK(target->type() == L"void (my_class::*)(int, int)");

    BOOST_CHECK_EQUAL(target->childs_size(), 0);

    auto args = res->child_at(1);
    BOOST_REQUIRE(args);
    BOOST_CHECK(args->name() == L"[args]");
    BOOST_CHECK(args->val() == L"<{ memvar=20 }, _1, 10>");
    BOOST_CHECK_EQUAL(args->childs_size(), 3);
}

/// Tests formatting libstdc++ bind result with member function pointer
BOOST_AUTO_TEST_CASE(stdcpp_mem_fun_ptr) {
    test_mem_fun_ptr(ctx, bind_kind::stdcpp);
}

/// Tests formatting libc++ bind result with member function pointer
BOOST_AUTO_TEST_CASE(libcxx_mem_fun_ptr) {
    test_mem_fun_ptr(ctx, bind_kind::libcxx);
}


/// Tests formatting bind result with function pointer and enabled raw data
void test_func_ptr_raw_data(context & ctx, bind_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    // creating function
    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    func->add_param(cm.bt_int());
    func->add_param(cm.bt_int());
    func->set_loc({cm.source("my_file.cpp"), 45});
    dbg.add_object(func, 0xA2, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    // creating std::_Bind type
    auto bind_obj = make_bind(knd, make_ptr(fptr_type, 0xAA), make_test_args(knd));
    ctx.set_show_raw_data(true);
    auto res = ctx.format(bind_obj);

    auto ph_name = knd == bind_kind::stdcpp ? L"std::_Placeholder<1>" : L"std::__1::placeholders::__ph<1>";

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"std::bind(&my_func + 8, _1, 10)");
    BOOST_CHECK(res->type() == val_type_name(bind_obj));
    BOOST_CHECK(res->val_pos().file() == "my_file.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 45);

    BOOST_REQUIRE(res->childs_size() == 3);

    auto target = res->child_at(0);
    BOOST_REQUIRE(target);
    BOOST_CHECK(target->name() == L"[target]");
    BOOST_CHECK(target->val() == L"0x00000000000000AA &my_func + 8");
    BOOST_CHECK(target->type() == L"void (*)(int, int)");

    BOOST_CHECK_EQUAL(target->childs_size(), 0);

    auto args = res->child_at(1);
    BOOST_REQUIRE(args);
    BOOST_CHECK(args->name() == L"[args]");
    BOOST_CHECK(args->val() == L"<_1, 10>");

    auto raw_data = res->child_at(2);
    BOOST_REQUIRE(raw_data);
    BOOST_CHECK(raw_data->name() == L"[Raw Data]");

}

/// Tests formatting libstdc++ bind result with function pointer
BOOST_AUTO_TEST_CASE(stdcpp_func_ptr_raw_data) {
    test_func_ptr_raw_data(ctx, bind_kind::stdcpp);
}

/// Tests formatting libc++ bind result with function pointer
BOOST_AUTO_TEST_CASE(libcxx_func_ptr_raw_data) {
    test_func_ptr_raw_data(ctx, bind_kind::libcxx);
}


/// Tests formatting bind result with lambda
void test_bind_lambda(context & ctx, bind_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    // creating lambda record type
    auto lambda_type  = cm.create_record();
    lambda_type->create_field("__x", cm.bt_int());
    auto lambda_call_op = lambda_type->create_method("operator()");
    lambda_call_op->set_ret_type(cm.bt_void());
    lambda_call_op->add_param(cm.bt_int());
    lambda_call_op->add_param(cm.bt_int());
    lambda_call_op->set_loc({cm.source("my_lambda.cpp"), 55});
    dbg.add_object(lambda_call_op, 0xA0, 100);
    dbg.make_def_rec_layout(lambda_type);

    // creating lambda record
    auto lambda = make_val(lambda_type);
    lambda["__x"] << 10;

    // creating std::_Bind type
    auto bind_obj = make_bind(knd, lambda, make_test_args(knd));
    auto res = ctx.format(bind_obj);

    auto ph_name = knd == bind_kind::stdcpp ? L"std::_Placeholder<1>" : L"std::__1::placeholders::__ph<1>";

    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"std::bind(&(anonymous record)::operator(), _1, 10)");
    BOOST_CHECK(res->type() == val_type_name(bind_obj));
    BOOST_CHECK(res->val_pos().file() == "my_lambda.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 55);

    BOOST_REQUIRE(res->childs_size() == 2);

    auto target = res->child_at(0);
    BOOST_REQUIRE(target);
    BOOST_CHECK(target->name() == L"[target]");
    BOOST_CHECK(target->val() == L"0x00000000000000A0 &(anonymous record)::operator()");
    BOOST_CHECK(target->type() == L"(unnamed struct)");

    BOOST_CHECK_EQUAL(target->childs_size(), 2);

    auto args = res->child_at(1);
    BOOST_REQUIRE(args);
    BOOST_CHECK(args->name() == L"[args]");
    BOOST_CHECK(args->val() == L"<_1, 10>");
}

/// Tests formatting libstdc++ bind result with lambda
BOOST_AUTO_TEST_CASE(stdcpp_bind_lambda) {
    test_bind_lambda(ctx, bind_kind::stdcpp);
}

/// Tests formatting libc++ bind result with lambda
BOOST_AUTO_TEST_CASE(libcxx_bind_lambda) {
    test_bind_lambda(ctx, bind_kind::libcxx);
}


BOOST_AUTO_TEST_SUITE_END()


}
