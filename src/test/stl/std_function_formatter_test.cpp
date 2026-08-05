
/// \file stdcpp_function_formatter_test.cpp
/// Contains unit tests for libstdc++ function formatters

#include "libcxx/libcxx_compressed_pair.hpp"
#include "stdcpp/stdcpp_bind.hpp"
#include "../test2.hpp"
#include <array>
#include <vector>
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {

namespace {
    enum function_kind {
        stdcpp,
        libcxx
    };
}


/// Creates CM for testing libstdc++ std::function
static cm::builder_result function_cm(cm::function_type * ftype, const cm::qual_type & functor_type) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    cm_builder b;

    const std::array<cm::qual_type, 1> adr = {b.typeref("stdcpp_any_data")};
    std::vector<cm::qual_type> invoker_params(adr.begin(), adr.end());
    invoker_params.insert(invoker_params.end(), ftype->params().begin(), ftype->params().end());
    auto invoker_type = b.ftype_r(ftype->ret_type(), invoker_params);

    // fake allocator type
    auto alloc_type = cm.find_named_record("my_allocator");
    if (!alloc_type) {
        alloc_type = cm.create_named_record("my_allocator", cm::record_kind::class_);
        dbg.make_def_rec_layout(alloc_type);
    }

    return b
        .ns("std")
            .record("_Any_data", "stdcpp_any_data")
                .ivar("_M_pod_data", b.atype(b.bt_char(), 16))
            .end()

            .record("_Function_base", "stdcpp_function_base")
                .templ("_Base_manager", "T").record("stdcpp_base_manager", functor_type)
                    .func("_M_manager", b.ftype(b.bt_bool(),
                                                b.rtype(b.typeref("stdcpp_any_data")),
                                                b.rtype(b.typeref("stdcpp_any_data")),
                                                b.bt_int()))
                .end().end()

                .ivar("_M_functor", b.typeref("stdcpp_any_data"))
                .ivar("_M_manager", b.ptype(b.ftype(b.bt_bool(),
                                                    b.rtype(b.typeref("stdcpp_any_data")),
                                                    b.rtype(b.typeref("stdcpp_any_data")),
                                                    b.bt_int())))
            .end()

            .templ("function", "T").record("stdcpp_function", ftype)
                .base(b.typeref("stdcpp_function_base"))
                .ivar("_M_invoker", b.ptype(invoker_type))
            .end().end()

            .ns("__1")
                .ns("__function")
                    .templ("__alloc_func", "Functor").record("libcxx_alloc_func", functor_type)
                        .ivar("__f_", libcxx_compressed_pair_type(functor_type, alloc_type))
                    .end().end()

                    .templ("__base", "FType").record("libcxx_base", ftype)
                    .end().end()

                    .templ("__func", "Functor").record("libcxx_func", functor_type)
                        .ivar("__f_", b.typeref("libcxx_alloc_func"))
                    .end().end()

                    .templ("__value_func", "FType").record("libcxx_value_func", ftype)
                        .ivar("__f_", b.ptype(b.typeref("libcxx_func")))
                    .end().end()
                .end()

                .templ("function", "FType").record("libcxx_function", ftype)
                    .ivar("__f_", b.typeref("libcxx_value_func"))
                .end().end()
            .end()
        .end()
    .build();
}


/// Returns type of manager function in CM for testing libstdc++ std::function
cm::function_type * stdcpp_function_m_manager_type(cm::function_type * ftype,
                                                   const cm::qual_type & functor_type) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto fcm = function_cm(ftype, functor_type);
    auto any_data_ref_type = cm.get_or_create_lvalue_ref_type(fcm.rtype("stdcpp_any_data"));
    return tctx.cm().get_or_create_func_type(cm.bt_bool(),
                                             any_data_ref_type,
                                             any_data_ref_type,
                                             cm.bt_int());
}


/// Returns type of pointer to manager function in CM for testing std::function
cm::pointer_type * stdcpp_function_m_manager_ptr_type(cm::function_type * ftype,
                                                      const cm::qual_type & functor_type) {
    auto & tctx = test_context::current_context();
    return tctx.cm().get_or_create_ptr_type(stdcpp_function_m_manager_type(ftype, functor_type));
}


/// Returns _M_manager function from CM created for testing std::function
cm::function * stdcpp_function_m_manager(cm::function_type * ftype,
                                         const cm::qual_type & functor_type) {
    auto fcm = function_cm(ftype, functor_type);

    auto base_manager_type = fcm.rtype("stdcpp_base_manager");
    auto m_manager = dynamic_cast<cm::function*>(base_manager_type->find_named_entity("_M_manager"));
    BOOST_REQUIRE(m_manager != nullptr);
    return m_manager;
}


/// Creates libstdc++ std::function object with specified
/// functor, manager, and invoker values
static value make_stdcpp_function(cm::function_type * ftype, const value & functor, const value & manager) {
    auto fcm = function_cm(ftype, val_type(functor));

    auto func = make_val(fcm.type("stdcpp_function"));

    func["_M_functor"].cast(functor.type()) << functor;
    func["_M_manager"] << manager;

    return func;
}


/// Creates libstdc++ std::function object with specified functor
static value make_stdcpp_function_functor(cm::function_type * ftype, const value & functor, bool is_big = false) {
    auto & tctx = test_context::current_context();
    auto fcm = function_cm(ftype, val_type(functor));

    // defining address of manager function
    auto m_manager = stdcpp_function_m_manager(ftype, val_type(functor));
    tctx.dbg().add_object(m_manager, 0xAA, 100);

    // creating pointer to manager
    auto manager_ptr_type = stdcpp_function_m_manager_ptr_type(ftype, val_type(functor));
    auto manager_ptr = make_ptr(manager_ptr_type, 0xAA);

    value functor_stored_val = is_big ? functor.ptr() : functor;

    return make_stdcpp_function(ftype, functor_stored_val, manager_ptr);
}


/// Makes libc++ std::function object with specified functor
static value make_libcxx_function_functor(cm::function_type * ftype, const value & functor) {
    auto & cm = test_context::current_context().cm();
    auto ftypes = function_cm(ftype, val_type(functor));

    // creating __func object
    auto alloc_type = cm.find_named_record("my_allocator");
    auto alloc = make_val(alloc_type);
    auto func_obj = make_val(ftypes.type("libcxx_func"));
    func_obj["__f_"]["__f_"] << make_libcxx_compressed_pair(functor, alloc);

    // creating std::function object
    auto func = make_val(ftypes.type("libcxx_function"));
    func["__f_"]["__f_"] << func_obj.ptr();
    return func;
}


/// Makes std::function object with specified functor
static value make_function_functor(function_kind knd,
                                   cm::function_type * ftype,
                                   const value & functor,
                                   bool is_big = false) {
    if (knd == function_kind::stdcpp) {
        return make_stdcpp_function_functor(ftype, functor, is_big);
    } else {
        return make_libcxx_function_functor(ftype, functor);
    }
}


/// Makes libc++ empty std::function object with specified functor
static value make_empty_libcxx_function(cm::function_type * ftype) {
    auto & tctx = test_context::current_context();
    auto ftypes = function_cm(ftype, tctx.cm().bt_int());

    // creating std::function object
    auto func = make_val(ftypes.type("libcxx_function"));
    func["__f_"]["__f_"] << make_null_to(ftypes.type("libcxx_base"));
    return func;
}


BOOST_FIXTURE_TEST_SUITE(std_function_formatter_test, fmt_test_fixture)


/// Checks result of formatting empty function
void check_empty(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[functor = <empty>]");
    BOOST_CHECK(res->type() == L"std::function<void()>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"[functor]");
    BOOST_CHECK(child->val() == L"<empty>");
    BOOST_CHECK(child->type() == L"");
    BOOST_CHECK_EQUAL(child->childs_size(), 0);
}

/// Tests formatting empty libstdc++ function
BOOST_AUTO_TEST_CASE(empty_stdcpp) {
    auto ftype = cm.get_or_create_func_type(cm.bt_void());
    auto fptr_type = cm.get_or_create_ptr_type(ftype);
    auto fcm = function_cm(ftype, fptr_type);
    auto manager = make_null(stdcpp_function_m_manager_ptr_type(ftype, fptr_type));
    auto func = make_stdcpp_function(ftype,
                                     make_null(fptr_type),
                                     manager);

    auto res = ctx.format(func);
    check_empty(res);
}

/// Tests formatting empty libc++ function
BOOST_AUTO_TEST_CASE(empty_libcxx) {
    auto ftype = cm.get_or_create_func_type(cm.bt_void());
    auto fptr_type = cm.get_or_create_ptr_type(ftype);
    auto fcm = function_cm(ftype, fptr_type);
    auto func = make_empty_libcxx_function(ftype);
    auto res = ctx.format(func);
    check_empty(res);
}


/// Tests formatting function with small functor
void test_small_functor(context & ctx, function_kind knd) {
    auto & cm = test_context::current_context().cm();

    cm_builder b;
    auto functor_type = b
        .record("my_functor", "my_functor")
            .ivar("x", b.bt_int())
            .ivar("y", b.bt_int())
        .end()
    .build().rtype("my_functor");

    auto my_functor = make_val(functor_type);
    my_functor["x"] << make_int(10);
    my_functor["y"] << make_int(20);

    auto ftype = cm.get_or_create_func_type(cm.bt_void());

    auto f = make_function_functor(knd, ftype, my_functor, false);

    auto res = ctx.format(f);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[functor = { x=10 y=20 }]");
    BOOST_CHECK(res->type() == L"std::function<void()>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"[functor]");
    BOOST_CHECK(child->val() == L"{ x=10 y=20 }");
    BOOST_CHECK(child->type() == L"my_functor");
    BOOST_CHECK_EQUAL(child->childs_size(), 2);
}

/// Tests formatting libstdc++ function with small functor
BOOST_AUTO_TEST_CASE(stdcpp_small_functor) {
    test_small_functor(ctx, function_kind::stdcpp);
}

/// Tests formatting libstdc++ function with small functor with disabled template parameter info
BOOST_AUTO_TEST_CASE(stdcpp_small_functor_no_template_params) {
    vctx.set_template_params_enabled(false);
    test_small_functor(ctx, function_kind::stdcpp);
}

/// Tests formatting libc++ function with small functor
BOOST_AUTO_TEST_CASE(libcxx_small_functor) {
    test_small_functor(ctx, function_kind::libcxx);
}


/// Tests formatting function with bug functor
void test_big_functor(context & ctx, function_kind knd) {
    auto & cm = test_context::current_context().cm();

    cm_builder b;
    auto functor_type = b
        .record("my_functor", "my_functor")
            .ivar("x", b.bt_long_long())
            .ivar("y", b.bt_long_long())
            .ivar("z", b.bt_long_long())
        .end()
    .build().rtype("my_functor");

    auto my_functor = make_val(functor_type);
    my_functor["x"] << make_long_long(10);
    my_functor["y"] << make_long_long(20);
    my_functor["z"] << make_long_long(30);

    auto ftype = cm.get_or_create_func_type(cm.bt_void());

    auto f = make_function_functor(knd, ftype, my_functor, true);
    auto res = ctx.format(f);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[functor = { x=10 y=20 z=30 }]");
    BOOST_CHECK(res->type() == L"std::function<void()>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"[functor]");
    BOOST_CHECK(child->val() == L"{ x=10 y=20 z=30 }");
    BOOST_CHECK(child->type() == L"my_functor");
    BOOST_CHECK_EQUAL(child->childs_size(), 3);
}

/// Tests formatting libstdc++ function with big functor
BOOST_AUTO_TEST_CASE(big_stdcpp_functor) {
    test_big_functor(ctx, function_kind::stdcpp);
}

/// Tests formatting libstdc++ function with big functor and with no template parameters info
BOOST_AUTO_TEST_CASE(big_stdcpp_functor_no_template_params) {
    vctx.set_template_params_enabled(false);
    test_big_functor(ctx, function_kind::stdcpp);
}

/// Tests formatting libc++ function with big functor
BOOST_AUTO_TEST_CASE(big_libcxx_functor) {
    test_big_functor(ctx, function_kind::libcxx);
}


/// Tests formatting function with result of std::bind
void test_bind_functor(context & ctx, function_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    // creating function
    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    func->add_param(cm.bt_int());
    func->add_param(cm.bt_int());
    dbg.add_object(func, 0xA2, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    // creating std::_Bind object
    auto bind_obj = make_stdcpp_bind(make_ptr(fptr_type, 0xA2), {make_int(10)});

    /// std::_Bind should always be considered outline in std::function storage
    /// because it's non trivial copyable
    auto f = make_function_functor(knd, ftype, bind_obj, true);

    auto res = ctx.format(f);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[functor = std::bind(&my_func, 10)]");
    BOOST_CHECK(res->type() == L"std::function<void(int, int)>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"[functor]");
    BOOST_CHECK(child->val() == L"std::bind(&my_func, 10)");
    BOOST_CHECK(child->type() == val_type_name(bind_obj));
    BOOST_CHECK_EQUAL(child->childs_size(), 2);
}

/// Tests foramtting libstdc++ function with result of std::bind
BOOST_AUTO_TEST_CASE(stdcpp_bind_functor) {
    test_bind_functor(ctx, function_kind::stdcpp);
}

/// Tests foramtting libstdc++ function with result of std::bind and with no template parameters info
BOOST_AUTO_TEST_CASE(stdcpp_bind_functor_no_template_params) {
    vctx.set_template_params_enabled(false);
    test_bind_functor(ctx, function_kind::stdcpp);
}

/// Tests foramtting libc++ function with result of std::bind
BOOST_AUTO_TEST_CASE(libcxx_bind_functor) {
    test_bind_functor(ctx, function_kind::libcxx);
}



/// Tests formatting function containing pointer to function
void test_function_pointer(context & ctx, function_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    auto func = cm.create_function("my_func");
    func->set_ret_type(cm.bt_void());
    func->set_loc({cm.source("test.cpp"), 33});
    dbg.add_object(func, 0xEE, 100);

    auto ftype = cm.get_or_create_func_type(func);
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    auto fptr = make_ptr(fptr_type, 0xEE);
    auto f = make_stdcpp_function_functor(ftype, fptr, false);

    auto res = ctx.format(f);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[functor = 0x00000000000000EE &my_func]");
    BOOST_CHECK(res->type() == L"std::function<void()>");
    BOOST_CHECK(res->val_pos().file() == "test.cpp");
    BOOST_CHECK_EQUAL(res->val_pos().line(), 33);
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"[functor]");
    BOOST_CHECK(child->val() == L"0x00000000000000EE &my_func");
    BOOST_CHECK(child->type() == L"void (*)()");
    BOOST_CHECK_EQUAL(child->childs_size(), 0);
}

/// Tests formatting libstdc++ std::function containing pointer to function
BOOST_AUTO_TEST_CASE(stdcpp_function_pointer) {
    test_function_pointer(ctx, function_kind::stdcpp);
}

/// Tests formatting libstdc++ std::function containing pointer to function
/// and with no template parameters info
BOOST_AUTO_TEST_CASE(stdcpp_function_pointer_no_template_parameters) {
    vctx.set_template_params_enabled(false);
    test_function_pointer(ctx, function_kind::stdcpp);
}

/// Tests formatting libc++ std::function containing pointer to function
BOOST_AUTO_TEST_CASE(libcxx_function_pointer) {
    test_function_pointer(ctx, function_kind::libcxx);
}


/// Tests formatting libstdc++ std::function containing pointer to member function
BOOST_AUTO_TEST_CASE(stdcpp_mem_fun_pointer) {
    auto cls_type = cm.create_named_record("my_record");
    auto cls_ptr_type = cm.get_or_create_ptr_type(cls_type);
    auto ftype = cm.get_or_create_func_type(cm.bt_void());
    auto fptr_type = cm.get_or_create_ptr_type(ftype);

    auto ftype_plain = cm.get_or_create_func_type(cm.bt_void(), cls_ptr_type);
    auto ftype_mem = cm.get_or_create_mem_ptr_type(cls_type, ftype);

    auto func = cls_type->create_method("my_func");
    func->set_ret_type(cm.bt_void());
    dbg.add_object(func, 0xEE, 100);

    auto rtype = cm.create_named_record("(anonymous struct)");
    rtype->create_field("__pfn", fptr_type);
    rtype->create_field("__delta", cm.bt_long());
    dbg.make_def_rec_layout(rtype);

    auto mem_fn = make_val(rtype);
    mem_fn["__pfn"] << make_ptr(fptr_type, 0xEE);
    mem_fn["__delta"] << make_long(0);

    auto fcm = function_cm(ftype_plain, val_type(mem_fn));

    cm_builder b;
    auto bres = b.ns("std")
        .templ("_Function_handler", "T1", "T2").record("fhandl", ftype_plain, ftype_mem)
            .func("_M_manager", b.ftype(b.bt_bool(),
                                        b.rtype(fcm.rtype("stdcpp_any_data")),
                                        b.rtype(fcm.rtype("stdcpp_any_data")),
                                        b.bt_int()))
        .end().end()
     .end()
    .build();

    auto fhandler_type = bres.rtype("fhandl");
    auto manager = dynamic_cast<cm::function*>(fhandler_type->find_named_entity("_M_manager"));
    BOOST_REQUIRE(manager != nullptr);

    dbg.add_object(manager, 0xEEE, 100);
    auto manager_val = make_ptr(cm.get_or_create_ptr_type(cm.get_or_create_func_type(manager)), 0xEEE);

    auto f = make_stdcpp_function(ftype_plain, mem_fn, manager_val);


    auto res = ctx.format(f);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[functor = 0x00000000000000EE &my_record::my_func]");
    BOOST_CHECK(res->type() == L"std::function<void(my_record*)>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"[functor]");
    BOOST_CHECK(child->val() == L"0x00000000000000EE &my_record::my_func");
    BOOST_CHECK(child->type() == L"void (my_record::*)()");
    BOOST_CHECK_EQUAL(child->childs_size(), 0);
}


/// Tests formatting libstdc++ std::function containing lambda represented with anonymous struct
BOOST_AUTO_TEST_CASE(stdcpp_lambda_anon) {
    auto cls_type = cm.create_named_record("my_record");
    auto cls_ptr_type = cm.get_or_create_ptr_type(cls_type);

    auto lambda_type = cm.create_record();
    auto call_op = lambda_type->create_method("operator()");
    call_op->set_ret_type(cm.bt_void());
    call_op->add_param(cm.bt_int());
    call_op->add_param(cm.bt_float());
    call_op->set_loc({cm.source("my_lambda.cpp"), 55});
    dbg.add_object(call_op, 0xA0, 100);
    dbg.make_def_rec_layout(lambda_type);

    auto ftype = cm.get_or_create_func_type(call_op);

    auto lambda = make_val(lambda_type);

    auto fcm = function_cm(ftype, val_type(lambda));

    cm_builder b;
    auto bres = b.ns("std")
        .templ("_Function_handler", "T1", "T2").record("fhandl", ftype, lambda_type)
            .func("_M_manager", b.ftype(b.bt_bool(),
                                        b.rtype(fcm.rtype("stdcpp_any_data")),
                                        b.rtype(fcm.rtype("stdcpp_any_data")),
                                        b.bt_int()))
        .end().end()
     .end()
    .build();

    auto fhandler_type = bres.rtype("fhandl");
    auto manager = dynamic_cast<cm::function*>(fhandler_type->find_named_entity("_M_manager"));

    dbg.add_object(manager, 0xEEE, 100);
    auto manager_val = make_ptr(cm.get_or_create_ptr_type(cm.get_or_create_func_type(manager)), 0xEEE);

    auto f = make_stdcpp_function(ftype, lambda, manager_val);


    auto res = ctx.format(f);
    BOOST_REQUIRE(res);

    BOOST_CHECK(res->val() == L"[functor = 0x00000000000000A0 &(anonymous record)::operator()]");
    BOOST_CHECK(res->type() == L"std::function<void(int, float)>");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto child = res->child_at(0);
    BOOST_REQUIRE(child);

    BOOST_CHECK(child->name() == L"[functor]");
    BOOST_CHECK(child->val() == L"0x00000000000000A0 &(anonymous record)::operator()");
    BOOST_CHECK(child->type() == L"(unnamed struct)");
}


BOOST_AUTO_TEST_SUITE_END()


}
