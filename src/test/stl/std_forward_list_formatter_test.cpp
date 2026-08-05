
/// \file std_forward_list_formatter_test.cpp
/// Contains unit tests for std::forward_list class.

#include "libcxx/libcxx_utils.hpp"
#include "stdcpp/stdcpp_utils.hpp"
#include "../test2.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>


namespace dbgfmt::test {


namespace {
    enum class flist_kind {
        stdcpp,
        libcxx
    };
}


/// Creates CM for forward list test with specified element type
static auto flist_cm(const cm::qual_type & elt_type) {
    cm_builder b;

    return b
        .ns("std")
            .record("_Fwd_list_node_base", "stdcpp_node_base")
                .ivar("_M_next", b.ptype(b.typeref("stdcpp_node_base")))
            .end()

            .templ("_Fwd_list_node", "T").record("stdcpp_node", elt_type)
                .base(b.typeref("stdcpp_node_base"))
                .ivar("_M_storage", get_stdcpp_aligned_buffer_type(elt_type))
            .end().end()

            .templ("_Fwd_list_base", "T").record("stdcpp_list_base", elt_type)
                .record("_Fwd_list_impl", "stdcpp_impl")
                    .ivar("_M_head", b.typeref("stdcpp_node_base"))
                .end()

                .ivar("_M_impl", b.typeref("stdcpp_impl"))
            .end().end()

            .templ("forward_list", "T").record("stdcpp_list", elt_type)
                .base(b.typeref("stdcpp_list_base"))
            .end().end()

            .templ("_Fwd_list_iterator", "T").record("stdcpp_iterator", elt_type)
                .ivar("_M_node", b.ptype(b.typeref("stdcpp_node_base")))
            .end().end()


            // libc++ forward_list
            .ns("__1")
                .templ("__forward_begin_node", "T").record("libcxx_begin_node", elt_type)
                    .ivar("__next_", b.ptype(b.typeref("libcxx_node")))
                .end().end()

                .templ("__forward_list_node", "T", "Ptr").record("libcxx_node", elt_type, b.ptype(b.bt_void()))
                    .base(b.typeref("libcxx_begin_node"))
                    .ivar("__value_", elt_type)
                .end().end()

                .record("__bb_pair", "bb_pair")
                    .ivar("__value_", b.typeref("libcxx_begin_node"))
                .end()

                .templ("__forward_list_base", "T").record("libcxx_list_base", elt_type)
                    .ivar("__before_begin_", b.typeref("bb_pair"))
                .end().end()

                .templ("forward_list", "T").record("libcxx_list", elt_type)
                    .base(b.typeref("libcxx_list_base"))
                .end().end()
            .end()
        .end()
    .build();
}


/// Returns type of forward list iterator
static cm::type_t * flist_iterator_type(flist_kind knd, const cm::qual_type & elt_type) {
    auto lcm = flist_cm(elt_type);
    if (knd == flist_kind::stdcpp) {
        return lcm.type("stdcpp_iterator");
    }

    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("__forward_list_iterator", "T").record("libcxx_iterator", b.ptype(lcm.type("libcxx_node")))
                .ivar("__ptr_", b.ptype(lcm.type("libcxx_begin_node")))
            .end().end()
        .end().end()
    .build().type("libcxx_iterator");
}


/// Creates node value for std::forward_list
static value create_flist_node(flist_kind knd, const value & val) {
    auto lcm = flist_cm(val_type(val));
    auto node_type_name = (knd == flist_kind::stdcpp) ? "stdcpp_node" : "libcxx_node";

    auto node_type = lcm.type(node_type_name);
    auto rec = make_val(node_type);

    if (knd == flist_kind::stdcpp) {
        rec["_M_storage"]["_M_storage"].cast(val.type()) << val;
    } else {
        rec["__value_"].cast(val.type()) << val;
    }

    return rec;
}


/// Creates std::forward list from range of values
template <typename Values>
static value create_flist_r(flist_kind knd, const cm::qual_type & elt_type, Values && vals) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto lcm = flist_cm(elt_type);

    auto base_name = (knd == flist_kind::stdcpp) ? "stdcpp_node_base" : "libcxx_begin_node";
    auto list_name = (knd == flist_kind::stdcpp) ? "stdcpp_list" : "libcxx_list";
    auto next_name = (knd == flist_kind::stdcpp) ? "_M_next" : "__next_";

    auto node_ptr = lcm.type(base_name)->ptr(cm);

    // creating nodes

    std::vector<value> nodes;
    nodes.reserve(vals.size());

    for (auto && val : vals) {
        auto node = create_flist_node(knd, val);
        if (!nodes.empty()) {
            nodes.back()[next_name] << node.ptr();
        }

        nodes.push_back(node);
    }

    if (!nodes.empty()) {
        nodes.back()[next_name] << make_null(node_ptr);
    }

    // creating list
    auto list = make_val(lcm.type(list_name));

    auto list_head = (knd == flist_kind::stdcpp) ?
                     list["_M_impl"]["_M_head"] :
                     list["__before_begin_"]["__value_"];

    if (!nodes.empty()) {
        list_head[next_name] << nodes.front().ptr();
    } else {
        list_head[next_name] << make_null(node_ptr);
    }

    return list;
}


/// Creates std::forward list from initializer list
static value create_flist_r(flist_kind knd, const cm::qual_type & elt_type, const std::initializer_list<value> & vals) {
    return create_flist_r<const std::initializer_list<value> &>(knd, elt_type, vals);
}


/// Creates std::forward_list from arguments
template <typename ... Vals>
static value create_flist(flist_kind knd, const cm::qual_type & elt_type, Vals && ... vals) {
    return create_flist_r(knd, elt_type, std::initializer_list<value>{vals...});
}


/// Creates iterator pointing to valuye
static value create_flist_iterator(flist_kind knd, const cm::qual_type & elt_type, const value & val) {
    auto node_ptr_name = (knd == flist_kind::stdcpp) ? "_M_node" : "__ptr_";

    auto lcm = flist_cm(elt_type);

    auto it = make_val(flist_iterator_type(knd, elt_type));
    it[node_ptr_name] << create_flist_node(knd, val).ptr();
    return it;
}


BOOST_FIXTURE_TEST_SUITE(stdcpp_forward_list, fmt_test_fixture)



/// Tests formatting empty forward_list
void test_empty_list(context & ctx, flist_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto res = ctx.format(create_flist_r(knd, cm.bt_int(), {}));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 0]");
    BOOST_CHECK(res->type() == L"std::forward_list<int>");

    BOOST_REQUIRE(res->childs_size() == 1);

    auto size = res->child_at(0);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"0");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);
}

/// Tests formatting empty libstdc++ forward_list
BOOST_AUTO_TEST_CASE(empty_stdcpp_list) {
    test_empty_list(ctx, flist_kind::stdcpp);
}

/// Tests formatting empty libc++ forward_list
BOOST_AUTO_TEST_CASE(empty_libcxx_list) {
    test_empty_list(ctx, flist_kind::libcxx);
}


/// Tests formatting simple forward list
void test_simple_list(context & ctx, flist_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto res = ctx.format(create_flist(knd, cm.bt_int(), make_int(10), make_int(20)));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 2]");
    BOOST_CHECK(res->type() == L"std::forward_list<int>");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"2");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto item0 = res->child_at(1);
    BOOST_REQUIRE(item0);
    BOOST_CHECK(item0->name() == L"[0]");
    BOOST_CHECK(item0->val() == L"10");
    BOOST_CHECK(item0->type() == L"int");
    BOOST_CHECK(item0->childs_size() == 0);

    auto item1 = res->child_at(2);
    BOOST_REQUIRE(item1);
    BOOST_CHECK(item1->name() == L"[1]");
    BOOST_CHECK(item1->val() == L"20");
    BOOST_CHECK(item1->type() == L"int");
    BOOST_CHECK(item1->childs_size() == 0);
}

/// Tests formatting simple libstdc++ forward_list
BOOST_AUTO_TEST_CASE(simple_stdcpp_list) {
    test_simple_list(ctx, flist_kind::stdcpp);
}

/// Tests formatting simple libc++ forward_list
BOOST_AUTO_TEST_CASE(simple_libcxx_list) {
    test_simple_list(ctx, flist_kind::libcxx);
}



/// Tests formatting simple forward list iterator
void test_simple_list_iterator(context & ctx, flist_kind knd) {
    auto & cm = test_context::current_context().cm();
    auto it = create_flist_iterator(knd, cm.bt_int(), make_int(10));

    auto node_fld_name = (knd == flist_kind::stdcpp) ? "_M_node" : "__ptr_";
    auto addr = it[node_fld_name].as_uint64() + 8;
    auto res = ctx.format(it);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10");
    if (knd == flist_kind::stdcpp) {
        BOOST_CHECK(res->type() == L"std::_Fwd_list_iterator<int>");
    } else {
        BOOST_CHECK(res->type() == L"std::__1::__forward_list_iterator<std::__1::__forward_list_node<int, void*>*>");
    }

    BOOST_REQUIRE(res->childs_size() == 1);

    auto ptr = res->child_at(0);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->val() == addr_str(addr) + L" 10");
    BOOST_CHECK(ptr->type() == L"int*");
    BOOST_REQUIRE(ptr->childs_size() == 1);

    auto val = ptr->child_at(0);
    BOOST_REQUIRE(val);
    BOOST_CHECK(val->name() == L"");
    BOOST_CHECK(val->val() == L"10");
    BOOST_CHECK(val->type() == L"int");
    BOOST_CHECK(val->childs_size() == 0);
}

/// Tests formatting simple libstdc++ forward_list iterator
BOOST_AUTO_TEST_CASE(simple_stdcpp_list_iterator) {
    test_simple_list_iterator(ctx, flist_kind::stdcpp);
}

/// Tests formatting simple libc++ forward_list iterator
BOOST_AUTO_TEST_CASE(simple_libcxx_list_iterator) {
    test_simple_list_iterator(ctx, flist_kind::libcxx);
}


BOOST_AUTO_TEST_SUITE_END()


}
