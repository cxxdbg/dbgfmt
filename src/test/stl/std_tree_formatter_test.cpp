
/// \file stdcpp_tree_test.cpp
/// Contains unit tests for libstdc++ tree containers formatters

#include "stdcpp/stdcpp_utils.hpp"
#include "stdcpp/stdcpp_val_utils2.hpp"
#include "libcxx/libcxx_utils.hpp"
#include "../test2.hpp"
#include "dbgfmt/context.hpp"
#include "../../template_name.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


enum class stdcpp_tree_format {
    old,        /// Old format (before c++11)
    cxx11,      /// C++11 format with __aligned_buffer
    cxx11_2     /// C++11 format with __aligned_membuf
};


/// Creates CM for testing libstc++ trees
static auto stdcpp_tree_cm(const cm::qual_type & key_type,
                           const cm::qual_type & elt_type,
                           stdcpp_tree_format fmt) {
    cm_builder b;

    b.ns("std")
        .record("_Rb_tree_node_base", "node_base")
            .ivar("_M_parent", b.ptype(b.typeref("node_base")))
            .ivar("_M_left", b.ptype(b.typeref("node_base")))
            .ivar("_M_right", b.ptype(b.typeref("node_base")))
        .end()
     .end();

    if (fmt == stdcpp_tree_format::cxx11_2) {
        b.ns("std")
            .templ("_Rb_tree_node", "T").record("node", elt_type)
                .base(b.typeref("node_base"))
                .ivar("_M_storage", get_stdcpp_aligned_membuf_type(elt_type))
            .end().end()
         .end();
    } else if (fmt == stdcpp_tree_format::cxx11) {
        b.ns("std")
            .templ("_Rb_tree_node", "T").record("node", elt_type)
                .base(b.typeref("node_base"))
                .ivar("_M_storage", get_stdcpp_aligned_buffer_type(elt_type))
            .end().end()
         .end();
    } else {
        b.ns("std")
            .templ("_Rb_tree_node", "T").record("node", elt_type)
                .base(b.typeref("node_base"))
                .ivar("_M_value_field", elt_type)
            .end().end()
         .end();
    }

    b.ns("std")
        .templ("_Rb_tree", "K", "T").record("tree", key_type, elt_type)
            .record("_Rb_tree_impl", "impl")
                .ivar("_M_header", b.typeref("node_base"))
                .ivar("_M_node_count", b.bt_unsigned_long())
            .end()

            .ivar("_M_impl", b.typeref("impl"))
        .end().end()

        .templ("_Rb_tree_iterator", "T").record("iterator", elt_type)
            .ivar("_M_node", b.ptype(b.typeref("node")))
        .end().end()
     .end();

    return b.build();
}


/// Constructs tree node with specified value and parent/left/right fields.
/// Return value containing pointer to constructed node
static value make_stdcpp_tree_node(const cm::qual_type & key_type,
                                   const value & v,
                                   const value & parent,
                                   const value & left,
                                   const value & right,
                                   stdcpp_tree_format fmt) {
    auto tcm = stdcpp_tree_cm(key_type, val_type(v), fmt);

    auto node = make_val(tcm.rtype("node"));
    node["_M_parent"] << parent;
    node["_M_left"] << left;
    node["_M_right"] << right;

    if (fmt == stdcpp_tree_format::cxx11_2) {
        node["_M_storage"]["_M_storage"].cast(v.type()) << v;
    } else if (fmt == stdcpp_tree_format::cxx11) {
        node["_M_storage"]["_M_storage"]["__data"].cast(v.type()) << v;
    } else {
        node["_M_value_field"] << v;
    }

    return cast_val(node, tcm.type("node_base")).ptr();
}


/// Makes tree with specified root node and size
static value make_stdcpp_tree(const cm::qual_type & key_type,
                              const cm::qual_type & elt_type,
                              const value & root_node,
                              unsigned long sz,
                              stdcpp_tree_format fmt) {

    auto tcm = stdcpp_tree_cm(key_type, elt_type, fmt);
    auto tree = make_val(tcm.rtype("tree"));
    tree["_M_impl"]["_M_header"]["_M_parent"] << root_node;
    tree["_M_impl"]["_M_node_count"] << make_ulong(sz);
    return tree;
}



/// Makes tree from 3 values
static value make_stdcpp_3_tree(const cm::qual_type & key_type,
                                const cm::qual_type & elt_type,
                                const value & v1,
                                const value & v2,
                                const value & v3,
                                stdcpp_tree_format fmt) {

    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto tcm = stdcpp_tree_cm(key_type, elt_type, fmt);

    auto null = make_null_to(tcm.rtype("node_base"));

    auto left = make_stdcpp_tree_node(key_type, v1, null, null, null, fmt);
    auto right = make_stdcpp_tree_node(key_type, v3, null, null, null, fmt);
    auto parent = make_stdcpp_tree_node(key_type, v2, null, left, right, fmt);

    return make_stdcpp_tree(key_type, elt_type, parent, 3, fmt);
}


/// Makes set container from 3 values
static value make_stdcpp_3_set_container(const std::string & name,
                                         const cm::qual_type & elt_type,
                                         const value & v1,
                                         const value & v2,
                                         const value & v3,
                                         stdcpp_tree_format fmt) {

    auto tcm = stdcpp_tree_cm(elt_type, elt_type, fmt);
    cm_builder b;
    auto ccm = b.ns("std")
        .templ(name, "T").record("cont", elt_type)
            .ivar("_M_t", tcm.type("tree"))
        .end().end()
     .end().build();

    auto cont = make_val(ccm.rtype("cont"));
    cont["_M_t"] << make_stdcpp_3_tree(elt_type, elt_type, v1, v2, v3, fmt);
    return cont;
}


/// Makes set container from 3 values
static value make_stdcpp_3_map_container(const std::string & name,
                                         const cm::qual_type & key_type,
                                         const cm::qual_type & val_type,
                                         const value & v1,
                                         const value & v2,
                                         const value & v3,
                                         stdcpp_tree_format fmt) {

    auto elt_type = get_stdcpp_pair_type(key_type, val_type);
    auto tcm = stdcpp_tree_cm(key_type, elt_type, fmt);
    cm_builder b;
    auto ccm = b.ns("std")
        .templ(name, "K", "V").record("cont", key_type, val_type)
            .ivar("_M_t", tcm.type("tree"))
        .end().end()
     .end().build();

    auto cont = make_val(ccm.rtype("cont"));
    cont["_M_t"] << make_stdcpp_3_tree(key_type, elt_type, v1, v2, v3, fmt);
    return cont;
}


/// Makes tree iterator pointing to specified node
static value make_stdcpp_tree_iterator(const cm::qual_type & key_type,
                                       const cm::qual_type & elt_type,
                                       const value & node,
                                       stdcpp_tree_format fmt) {

    auto tcm = stdcpp_tree_cm(key_type, elt_type, fmt);
    auto it = make_val(tcm.rtype("iterator"));
    it["_M_node"] << node;
    return it;
}


/// Creates CM for testing libc++ tree
static auto libcxx_tree_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("__tree_node_base", "Ptr").record("node_base", b.ptype(b.bt_void()))
                .ivar("__left_", b.ptype(b.typeref("node_base")))
                .ivar("__right_", b.ptype(b.typeref("node_base")))
                .ivar("__parent_", b.ptype(b.typeref("end_node")))
            .end().end()

            .templ("__tree_node", "T", "Ptr").record("node", elt_type, b.ptype(b.bt_void()))
                .base(b.typeref("node_base"))
                .ivar("__value_", elt_type)
            .end().end()

            .record("__tree_end_node", "end_node")
                .ivar("__left_", b.ptype(b.typeref("node_base")))
            .end()

            .record({}, "end_node_pair")
                .ivar("__value_", b.typeref("end_node"))
            .end()

            .record({}, "size_pair")
                .ivar("__value_", b.bt_unsigned_long())
            .end()

            .templ("__tree", "T").record("tree", elt_type)
                .ivar("__begin_node_", b.ptype(b.typeref("end_node")))
                .ivar("__pair1_", b.typeref("end_node_pair"))
                .ivar("__pair3_", b.typeref("size_pair"))
            .end().end()

            // .templ("__tree_const_iterator", "Val", "NodePtr")
            //     .record("tree_const_iterator", elt_type, b.ptype(b.typeref("node")))
            //         .ivar("__ptr_", b.ptype(b.typeref("end_node")))
            //     .end()
            // .end()

            // .templ("__tree_iterator", "Val", "NodePtr")
            //     .record("tree_iterator", elt_type, b.ptype(b.typeref("node")))
            //         .ivar("__ptr_", b.ptype(b.typeref("end_node")))
            //     .end()
            // .end()

            // .templ("__map_iterator", "TreeIt").record("map_iterator", b.typeref("tree_iterator"))
            //     .ivar("__i_", b.typeref("tree_iterator"))
            // .end().end()
        .end().end()
    .build();
}



/// Constructs libc++tree node with specified value and parent/left/right fields.
static value make_libcxx_tree_node(const value & v,
                                   const value & parent,
                                   const value & left,
                                   const value & right) {
    auto tcm = libcxx_tree_cm(val_type(v));

    auto node = make_val(tcm.rtype("node"));
    node["__parent_"] << parent;
    node["__left_"] << left;
    node["__right_"] << right;
    node["__value_"] << v;
    return cast_val(node, tcm.type("node_base")).ptr();
}


/// Makes libc++ tree from 3 values
static value make_libcxx_3_tree(const cm::qual_type & elt_type,
                                const value & v1,
                                const value & v2,
                                const value & v3) {

    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto tcm = libcxx_tree_cm(elt_type);

    auto null = make_null_to(tcm.rtype("node_base"));

    auto tree = make_val(tcm.rtype("tree"));

    auto left = make_libcxx_tree_node(v1, null, null, null);
    auto right = make_libcxx_tree_node(v3, null, null, null);

    auto parent = make_libcxx_tree_node(v2, tree["__pair1_"]["__value_"].ptr(), left, right);
    tree["__pair1_"]["__value_"]["__left_"] << parent;

    cast_val(*left, tcm.rtype("node"))["__parent_"] << parent;
    cast_val(*right, tcm.rtype("node"))["__parent_"] << parent;

    tree["__begin_node_"] << left;
    tree["__pair3_"]["__value_"] << make_ulong(3);

    return tree;
}


/// Makes set container from 3 values
static value make_libcxx_3_set_container(const std::string & name,
                                         const cm::qual_type & elt_type,
                                         const value & v1,
                                         const value & v2,
                                         const value & v3) {

    auto tcm = libcxx_tree_cm(elt_type);
    cm_builder b;
    auto ccm =
    b.ns("std").ns("__1")
        .templ(name, "T").record("cont", elt_type)
            .ivar("__tree_", tcm.type("tree"))
        .end().end()
    .end().end().build();

    auto cont = make_val(ccm.rtype("cont"));
    cont["__tree_"] << make_libcxx_3_tree(elt_type, v1, v2, v3);
    return cont;
}


/// Makes set libc++ container from 3 values
static value make_libcxx_3_map_container(const std::string & name,
                                         const cm::qual_type & key_type,
                                         const cm::qual_type & val_type,
                                         const value & v1,
                                         const value & v2,
                                         const value & v3) {

    auto pair_type = get_libcxx_pair_type(key_type, val_type);

    cm_builder val_b;
    auto val_cm = val_b.ns("std").ns("__1")
        .templ("__value_type", "First", "Second").record("value_type", key_type, val_type)
            .ivar("__cc", pair_type)
        .end().end()
    .end().end().build();

    auto elt_type = val_cm.rtype("value_type");

    auto tcm = libcxx_tree_cm(elt_type);
    cm_builder b;
    auto ccm =
    b.ns("std").ns("__1")
        .templ(name, "Key", "Val").record("cont", key_type, val_type)
            .ivar("__tree_", tcm.type("tree"))
        .end().end()
    .end().end().build();

    auto cont = make_val(ccm.rtype("cont"));
    cont["__tree_"] << make_libcxx_3_tree(val_type, v1, v2, v3);
    return cont;
}



BOOST_FIXTURE_TEST_SUITE(stdcpp_tree_test, fmt_test_fixture)


static void check_simple_set_cont(const fmt_result_ref & res, const std::string & name) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 3]");
    std::wstring wname{name.begin(), name.end()};
    auto tname = std::wstring{L"std::"} + wmake_template_inst_name(wname, {L"int"});
    BOOST_CHECK(res->type() == tname);

    BOOST_REQUIRE(res->childs_size() == 4);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"3");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK(sz->childs_size() == 0);

    auto c1 = res->child_at(1);
    BOOST_REQUIRE(c1);
    BOOST_CHECK(c1->name() == L"[0]");
    BOOST_CHECK(c1->val() == L"10");
    BOOST_CHECK(c1->type() == L"int");
    BOOST_CHECK(c1->childs_size() == 0);

    auto c2 = res->child_at(2);
    BOOST_REQUIRE(c2);
    BOOST_CHECK(c2->name() == L"[1]");
    BOOST_CHECK(c2->val() == L"20");
    BOOST_CHECK(c2->type() == L"int");
    BOOST_CHECK(c2->childs_size() == 0);

    auto c3 = res->child_at(3);
    BOOST_REQUIRE(c3);
    BOOST_CHECK(c3->name() == L"[2]");
    BOOST_CHECK(c3->val() == L"30");
    BOOST_CHECK(c3->type() == L"int");
    BOOST_CHECK(c3->childs_size() == 0);
}


/// Tests displaying simple libstdc++ set container with specified name and std
static void test_simple_stdcpp_set_cont(context & ctx, const std::string & name, stdcpp_tree_format fmt) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto set = make_stdcpp_3_set_container(name, cm.bt_int(), make_int(10), make_int(20), make_int(30), fmt);
    auto res = ctx.format(set);

    check_simple_set_cont(res, name);
}


/// Tests displaying simple std::set for old std
BOOST_AUTO_TEST_CASE(simple_stdcpp_set_oldstd) {
    test_simple_stdcpp_set_cont(ctx, "set", stdcpp_tree_format::old);
}

/// Tests displaying simple std::set for new std
BOOST_AUTO_TEST_CASE(simple_stdcpp_set_newstd) {
    test_simple_stdcpp_set_cont(ctx, "set", stdcpp_tree_format::cxx11);
}

/// Tests displaying simple std::set for c++11 v2
BOOST_AUTO_TEST_CASE(simple_stdcpp_set_cxx11_2) {
    test_simple_stdcpp_set_cont(ctx, "set", stdcpp_tree_format::cxx11_2);
}


/// Tests displaying simple libc++ set container with specified name and std
static void test_simple_libcxx_set_cont(context & ctx, const std::string & name) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto set = make_libcxx_3_set_container(name, cm.bt_int(), make_int(10), make_int(20), make_int(30));
    auto res = ctx.format(set);

    check_simple_set_cont(res, name);
}


/// Tests displaying simple std::set for old std
BOOST_AUTO_TEST_CASE(simple_libcxx_set) {
    test_simple_libcxx_set_cont(ctx, "set");
}


/// Checks result of formatting simple map container with specified name and std
static void check_simple_map_cont(const fmt_result_ref & res,
                                  const std::string & name,
                                  const std::wstring & elt_tname) {

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 3]");
    std::wstring wname{name.begin(), name.end()};
    auto tname = std::wstring{L"std::"} + wmake_template_inst_name(wname, {L"int", L"int"});
    BOOST_CHECK(res->type() == tname);

    BOOST_REQUIRE(res->childs_size() == 4);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"3");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK(sz->childs_size() == 0);

    auto c1 = res->child_at(1);
    BOOST_REQUIRE(c1);
    BOOST_CHECK(c1->name() == L"[0]");
    BOOST_CHECK(c1->val() == L"{ first=10 second=50 }");
    BOOST_CHECK(c1->type() == elt_tname);
    BOOST_CHECK(c1->childs_size() == 2);

    auto c2 = res->child_at(2);
    BOOST_REQUIRE(c2);
    BOOST_CHECK(c2->name() == L"[1]");
    BOOST_CHECK(c2->val() == L"{ first=20 second=60 }");
    BOOST_CHECK(c2->type() == elt_tname);
    BOOST_CHECK(c2->childs_size() == 2);

    auto c3 = res->child_at(3);
    BOOST_REQUIRE(c3);
    BOOST_CHECK(c3->name() == L"[2]");
    BOOST_CHECK(c3->val() == L"{ first=30 second=70 }");
    BOOST_CHECK(c3->type() == elt_tname);
    BOOST_CHECK(c3->childs_size() == 2);
}


/// Tests displaying simple map container with specified name and std
static void test_simple_stdcpp_map_cont(context & ctx, const std::string & name, stdcpp_tree_format fmt) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto map = make_stdcpp_3_map_container(name,
                                           cm.bt_int(),
                                           cm.bt_int(),
                                           make_pair(make_int(10), make_int(50)),
                                           make_pair(make_int(20), make_int(60)),
                                           make_pair(make_int(30), make_int(70)),
                                           fmt);
    auto res = ctx.format(map);
    check_simple_map_cont(res, name, L"std::pair<int, int>");
}


/// Tests displaying simple std::map for old std
BOOST_AUTO_TEST_CASE(simple_stdcpp_map_oldstd) {
    test_simple_stdcpp_map_cont(ctx, "map", stdcpp_tree_format::old);
}

/// Tests displaying simple std::map for new std
BOOST_AUTO_TEST_CASE(simple_stdcpp_map_newstd) {
    test_simple_stdcpp_map_cont(ctx, "map", stdcpp_tree_format::cxx11);
}

/// Tests displaying simple std::map for c++11 v2
BOOST_AUTO_TEST_CASE(simple_stdcpp_map_cxx11_2) {
    test_simple_stdcpp_map_cont(ctx, "map", stdcpp_tree_format::cxx11_2);
}

/// Tests displaying simple libc++ map container with specified name
static void test_simple_libcxx_map_cont(context & ctx, const std::string & name) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto map = make_libcxx_3_map_container(name,
                                           cm.bt_int(),
                                           cm.bt_int(),
                                           make_libcxx_pair(make_int(10), make_int(50)),
                                           make_libcxx_pair(make_int(20), make_int(60)),
                                           make_pair(make_int(30), make_int(70)));
    auto res = ctx.format(map);
    check_simple_map_cont(res, name, L"std::__1::pair<int, int>");
}

/// Tests displaying simple std::map for libc++
BOOST_AUTO_TEST_CASE(simple_libcxx_map) {
    test_simple_libcxx_map_cont(ctx, "map");
}



/// Tests displaying simple std::multiset for old std
BOOST_AUTO_TEST_CASE(simple_stdcpp_multiset_oldstd) {
    test_simple_stdcpp_set_cont(ctx, "multiset", stdcpp_tree_format::old);
}

/// Tests displaying simple std::multiset for new std
BOOST_AUTO_TEST_CASE(simple_stdcpp_multiset_newstd) {
    test_simple_stdcpp_set_cont(ctx, "multiset", stdcpp_tree_format::cxx11);
}

/// Tests displaying simple std::multiset for c++11 v2
BOOST_AUTO_TEST_CASE(simple_stdcpp_multiset_cxx11_2) {
    test_simple_stdcpp_set_cont(ctx, "multiset", stdcpp_tree_format::cxx11_2);
}

/// Tests displaying simple std::multiset for libc++
BOOST_AUTO_TEST_CASE(simple_libcxx_multiset) {
    test_simple_libcxx_set_cont(ctx, "multiset");
}


/// Tests displaying simple std::multimap for old std
BOOST_AUTO_TEST_CASE(simple_stdcpp_multimap_oldstd) {
    test_simple_stdcpp_map_cont(ctx, "multimap", stdcpp_tree_format::old);
}

/// Tests displaying simple std::multimap for new std
BOOST_AUTO_TEST_CASE(simple_stdcpp_multimap_newstd) {
    test_simple_stdcpp_map_cont(ctx, "multimap", stdcpp_tree_format::cxx11);
}

/// Tests displaying simple std::multimap for c++11 v2
BOOST_AUTO_TEST_CASE(simple_stdcpp_multimap_cxx11_v2) {
    test_simple_stdcpp_map_cont(ctx, "multimap", stdcpp_tree_format::cxx11_2);
}

/// Tests displaying simple std::multimap for libc++
BOOST_AUTO_TEST_CASE(simple_libcxx_multimap) {
    test_simple_libcxx_map_cont(ctx, "multimap");
}


/// Checks result of formatting tree iterator
static void check_iterator(const fmt_result_ref & res, const std::wstring & type_name) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10");
    BOOST_CHECK(res->type() == type_name);

    BOOST_REQUIRE(res->childs_size() == 1);
    auto ptr = res->child_at(0);
    BOOST_REQUIRE(ptr);
    BOOST_CHECK(ptr->name() == L"[ptr]");
    BOOST_CHECK(ptr->type() == L"int*");

    BOOST_REQUIRE(ptr->childs_size() == 1);
    auto val = ptr->child_at(0);
    BOOST_REQUIRE(val);
    BOOST_CHECK(val->name() == L"");
    BOOST_CHECK(val->val() == L"10");
    BOOST_CHECK(val->type() == L"int");
    BOOST_CHECK(val->childs_size() == 0);
}


/// Tests displaying libstdc++ tree iterator
BOOST_AUTO_TEST_CASE(stdcpp_iterator) {
    auto tcm = stdcpp_tree_cm(cm.bt_int(), cm.bt_int(), stdcpp_tree_format::cxx11);
    auto node_base_ptr_type = cm.get_or_create_ptr_type(tcm.type("node_base"));
    auto node_ptr_type = cm.get_or_create_ptr_type(tcm.type("node"));
    auto null = make_null_to(tcm.type("node"));

    auto node = make_stdcpp_tree_node(cm.bt_int(),
                                      make_int(10),
                                      null,
                                      null,
                                      null,
                                      stdcpp_tree_format::cxx11);
    auto cnode = cast_val(node, node_ptr_type);

    auto it = make_stdcpp_tree_iterator(cm.bt_int(), cm.bt_int(), cnode, stdcpp_tree_format::cxx11);
    auto res = ctx.format(it);
    check_iterator(res, L"std::_Rb_tree_iterator<int>");
}


/// Tests displaying libc++ tree const iterator
BOOST_AUTO_TEST_CASE(libcxx_const_iterator) {
    auto tcm = libcxx_tree_cm(cm.bt_int());
    auto end_node_ptr_type = cm.get_or_create_ptr_type(tcm.rtype("end_node"));

    cm_builder b;
    auto icm = b.ns("std").ns("__1")
        .templ("__tree_const_iterator", "Val", "NodePtr")
            .record("tree_const_iterator", b.bt_int(), b.ptype(tcm.rtype("node")))
                .ivar("__ptr_", b.ptype(tcm.rtype("end_node")))
            .end()
        .end()
    .end().end().build();

    auto null = make_null_to(tcm.type("node"));
    auto node = make_libcxx_tree_node(make_int(10),
                                      null,
                                      null,
                                      null);
    auto cnode = cast_val(node, end_node_ptr_type);

    auto it = make_val(icm.rtype("tree_const_iterator"));
    it["__ptr_"] << cnode;

    auto res = ctx.format(it);
    check_iterator(res, L"std::__1::__tree_const_iterator<int, std::__1::__tree_node<int, void*>*>");
}


/// Tests displaying libc++ tree iterator
BOOST_AUTO_TEST_CASE(libcxx_iterator) {
    auto tcm = libcxx_tree_cm(cm.bt_int());
    auto end_node_ptr_type = cm.get_or_create_ptr_type(tcm.rtype("end_node"));

    cm_builder b;
    auto icm = b.ns("std").ns("__1")
        .templ("__tree_iterator", "Val", "NodePtr")
            .record("tree_iterator", b.bt_int(), b.ptype(tcm.rtype("node")))
                .ivar("__ptr_", b.ptype(tcm.rtype("end_node")))
            .end()
        .end()
    .end().end().build();

    auto null = make_null_to(tcm.type("node"));
    auto node = make_libcxx_tree_node(make_int(10),
                                      null,
                                      null,
                                      null);
    auto cnode = cast_val(node, end_node_ptr_type);

    auto it = make_val(icm.rtype("tree_iterator"));
    it["__ptr_"] << cnode;

    auto res = ctx.format(it);
    check_iterator(res, L"std::__1::__tree_iterator<int, std::__1::__tree_node<int, void*>*>");
}


/// Tests displaying libc++ map iterator
BOOST_AUTO_TEST_CASE(libcxx_map_iterator) {
    auto tcm = libcxx_tree_cm(cm.bt_int());
    auto end_node_ptr_type = cm.get_or_create_ptr_type(tcm.rtype("end_node"));

    cm_builder b;
    auto icm = b.ns("std").ns("__1")
        .templ("__tree_iterator", "Val", "NodePtr")
            .record("tree_iterator", b.bt_int(), b.ptype(tcm.rtype("node")))
                .ivar("__ptr_", b.ptype(tcm.rtype("end_node")))
            .end()
        .end()
    .end().end().build();

    cm_builder b2;
    auto micm = b2.ns("std").ns("__1")
        .templ("__map_iterator", "TreeIt").record("map_iterator", icm.rtype("tree_iterator"))
            .ivar("__i_", icm.rtype("tree_iterator"))
        .end().end()
    .end().end().build();

    auto null = make_null_to(tcm.type("node"));
    auto node = make_libcxx_tree_node(make_int(10),
                                      null,
                                      null,
                                      null);
    auto cnode = cast_val(node, end_node_ptr_type);

    auto it = make_val(micm.rtype("map_iterator"));
    it["__i_"]["__ptr_"] << cnode;

    auto res = ctx.format(it);
    check_iterator(res, L"std::__1::__tree_iterator<int, std::__1::__tree_node<int, void*>*>");
}


BOOST_AUTO_TEST_SUITE_END()


}
