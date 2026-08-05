
/// \file stdcpp_hashtable_formatter_test.cpp
/// Contains unit tests for libstdc++ hash table formatter.

#include "libcxx/libcxx_utils.hpp"
#include "stdcpp/stdcpp_val_utils2.hpp"
#include "../test2.hpp"
#include "dbgfmt/context.hpp"
#include "../../template_name.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Builds CM required for libstdc++ hash table tests
static auto stdcpp_hash_table_cm(const std::string & cont_name,
                                 bool single_par,
                                 const cm::qual_type & key_type,
                                 const cm::qual_type & val_type) {

    cm::qual_type ht_val_type = single_par ?
                                key_type :
                                get_stdcpp_pair_type(key_type, val_type);

    cm_builder b;
    b.ns("std")
            .record("_Hash_node_base", "node_base")
                .ivar("_M_nxt", b.ptype(b.typeref("node_base")))
            .end()

            .templ("_Hash_node_value_base", "T").record("node", ht_val_type)
                .base(b.typeref("node_base"))
                .ivar("_M_storage", get_stdcpp_aligned_buffer_type(ht_val_type))
            .end().end()

            .templ("_Hashtable", "K", "T").record("hashtable", key_type, ht_val_type)
                .ivar("_M_before_begin", b.typeref("node_base"))
                .ivar("_M_element_count", b.bt_unsigned_long())
            .end().end()
     .end();

    if (single_par) {
        b.ns("std")
            .templ(cont_name, "T").record("cont", key_type)
                .ivar("_M_h", b.typeref("hashtable"))
            .end()
         .end();
    } else {
        b.ns("std")
            .templ(cont_name, "K", "V").record("cont", key_type, val_type)
                .ivar("_M_h", b.typeref("hashtable"))
            .end()
         .end();
    }

    return b.build();
}


/// Creates libstdc++ hashtable container record with specified
/// key/value types and vector of values
static value make_stdcpp_hash_table_cont(const std::string & cont_name,
                                         bool is_single_par,
                                         const cm::qual_type & key_type,
                                         const cm::qual_type & val_type,
                                         const std::vector<value> & vals) {

    auto hcm = stdcpp_hash_table_cm(cont_name, is_single_par, key_type, val_type);
    auto & tctx = test_context::current_context();

    auto cont_type = hcm.type("cont");
    auto node_type = hcm.type("node");

    // creating nodes
    std::vector<value> nodes;
    for (auto && val : vals) {
        auto node = make_val(node_type);
        node["_M_storage"]["_M_storage"].cast(val.type()) << val;

        if (!nodes.empty()) {
            nodes.back()["_M_nxt"] << node.ptr();
        }

        nodes.push_back(node);
    }

    // creating hash table container
    auto res = make_val(cont_type);
    if (!nodes.empty()) {
        res["_M_h"]["_M_before_begin"]["_M_nxt"] << nodes.front().ptr();
    } else {
        res["_M_h"]["_M_before_begin"]["_M_nxt"] << make_null_to(node_type);
    }

    res["_M_h"]["_M_element_count"] << make_ulong(nodes.size());
    return res;
}


/// Builds CM for testing libc++ hash table
static auto libcxx_hash_table_cm(const cm::qual_type & elt_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .record("__hash_node_base", "hash_node_base")
                .ivar("__next_", b.ptype(b.typeref("hash_node_base")))
            .end()

            .templ("__hash_node", "T", "Ptr").record("hash_node", elt_type, b.ptype(b.bt_void()))
                .base(b.typeref("hash_node_base"))
                .ivar("__hash_", b.bt_unsigned_long())
                .ivar("__value_", elt_type)
            .end().end()

            .record("__compressed_pair_hash_node_base", "hash_node_base_cp")
                .ivar("__value_", b.typeref("hash_node_base"))
            .end()

            .templ("__hash_table", "T").record("hash_table", elt_type)
                .ivar("__p1_", b.typeref("hash_node_base_cp"))
                .ivar("__p2_", get_libcxx_compressed_pair_type(b.bt_unsigned_long()))
            .end().end()
        .end().end()
    .build();
}


/// Gets or creates libc++ hash_value_type
static cm::type_t * get_libcxx_hash_value_type(const cm::qual_type & key_type, const cm::qual_type & val_type) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("__hash_value_type", "Key", "Value").record("hash_value_type", key_type, val_type)
                .ivar("__cc", get_libcxx_pair_type(key_type, val_type))
            .end().end()
        .end().end()
    .build().type("hash_value_type");
}


/// Gets or creates libc++ map container type
static cm::type_t * get_libcxx_map_type(const std::string & cont_name,
                                        const cm::qual_type & key_type,
                                        const cm::qual_type & val_type) {


    auto hv_type = get_libcxx_hash_value_type(key_type, val_type);
    auto ht_type = libcxx_hash_table_cm(hv_type).type("hash_table");

    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ(cont_name, "Key", "Val").record("container", key_type, val_type)
                .ivar("__table_", ht_type)
            .end().end()
        .end().end()
    .build().type("container");
}


/// Gets or creates libc++ set container type
static cm::type_t * get_libcxx_set_type(const std::string & cont_name, const cm::qual_type & key_type) {

    auto ht_type = libcxx_hash_table_cm(key_type).type("hash_table");

    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ(cont_name, "Key").record("container", key_type)
                .ivar("__table_", ht_type)
            .end().end()
        .end().end()
    .build().type("container");
}


/// Makes libc++ hash table container of specified type
static value make_libcxx_hash_table_cont(const cm::qual_type & cont_type,
                                         const cm::qual_type & elt_type,
                                         const std::vector<value> & vals) {
    auto node_type = libcxx_hash_table_cm(elt_type).type("hash_node");
    auto res = make_val(cont_type);
    auto prev_node = res["__table_"]["__p1_"]["__value_"];

    for (auto && val : vals) {
        auto node = make_val(node_type);
        node["__value_"] << val;
        prev_node["__next_"] << node.ptr();
        prev_node = node;
    }

    prev_node["__next_"] << make_null_to(node_type);
    res["__table_"]["__p2_"]["__value_"] << vals.size();
    return res;
}


/// Makes libc++ set container
static value make_libcxx_set(const std::string & cont_name,
                             const cm::qual_type & elt_type,
                             const std::vector<value> & vals) {
    auto cont_type = get_libcxx_set_type(cont_name, elt_type);
    return make_libcxx_hash_table_cont(cont_type, elt_type, vals);
}


/// Makes libc++ map container
static value make_libcxx_map(const std::string & cont_name,
                             const cm::qual_type & key_type,
                             const cm::qual_type & val_type,
                             const std::vector<value> & vals) {
    auto elt_type = get_libcxx_hash_value_type(key_type, val_type);
    auto cont_type = get_libcxx_map_type(cont_name, key_type, val_type);
    return make_libcxx_hash_table_cont(cont_type, elt_type, vals);
}



BOOST_FIXTURE_TEST_SUITE(stdcpp_hashtable_formatter_test, fmt_test_fixture)


/// Checks result of formatting simple set container
void check_simple_set(const fmt_result_ref & res, const std::string & cont_name) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 3]");
    std::wstring wcont_name{cont_name.begin(), cont_name.end()};
    const auto & tiname = std::wstring(L"std::") +
            wmake_template_inst_name(wcont_name, {L"int"});
    BOOST_CHECK(res->type() == tiname);

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

/// Tests formatting simple libstdc++ set container with specified name
static void test_simple_stdcpp_set(context & ctx, const std::string & cont_name) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    std::vector<value> vals;
    vals.push_back(make_int(10));
    vals.push_back(make_int(20));
    vals.push_back(make_int(30));
    auto val = make_stdcpp_hash_table_cont(cont_name, true, cm.bt_int(), cm.bt_int(), vals);
    auto res = ctx.format(val);
    check_simple_set(res, cont_name);
}

/// Tests formatting simple libc++ set container with specified name
static void test_simple_libcxx_set(context & ctx, const std::string & cont_name) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    std::vector<value> vals;
    vals.push_back(make_int(10));
    vals.push_back(make_int(20));
    vals.push_back(make_int(30));
    auto val = make_libcxx_set(cont_name, cm.bt_int(), vals);
    auto res = ctx.format(val);
    check_simple_set(res, cont_name);
}

BOOST_AUTO_TEST_CASE(test_stdcpp_set) {
    test_simple_stdcpp_set(ctx, "unordered_set");
}

BOOST_AUTO_TEST_CASE(test_stdcpp_multiset) {
    test_simple_stdcpp_set(ctx, "unordered_multiset");
}

BOOST_AUTO_TEST_CASE(test_libcxx_set) {
    test_simple_libcxx_set(ctx, "unordered_set");
}

BOOST_AUTO_TEST_CASE(test_libcxx_multiset) {
    test_simple_libcxx_set(ctx, "unordered_multiset");
}


/// Checks result of formatting simple map container
void check_simple_map(const fmt_result_ref & res, const std::string & cont_name, const std::wstring & pair_name) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 3]");
    std::wstring wcont_name{cont_name.begin(), cont_name.end()};
    auto winame = std::wstring(L"std::") +
            wmake_template_inst_name(wcont_name, {L"int", L"int"});
    BOOST_CHECK(res->type() == winame);

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
    BOOST_CHECK(c1->type() == pair_name + L"<int, int>");
    BOOST_CHECK(c1->childs_size() == 2);

    auto c2 = res->child_at(2);
    BOOST_REQUIRE(c2);
    BOOST_CHECK(c2->name() == L"[1]");
    BOOST_CHECK(c2->val() == L"{ first=20 second=60 }");
    BOOST_CHECK(c2->type() == pair_name + L"<int, int>");
    BOOST_CHECK(c2->childs_size() == 2);

    auto c3 = res->child_at(3);
    BOOST_REQUIRE(c3);
    BOOST_CHECK(c3->name() == L"[2]");
    BOOST_CHECK(c3->val() == L"{ first=30 second=70 }");
    BOOST_CHECK(c3->type() == pair_name + L"<int, int>");
    BOOST_CHECK(c3->childs_size() == 2);
}

/// Tests formatting simple libstdc++ map container with specified name
static void test_simple_stdcpp_map(context & ctx, const std::string & cont_name) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    std::vector<value> vals;
    vals.push_back(make_pair(make_int(10), make_int(50)));
    vals.push_back(make_pair(make_int(20), make_int(60)));
    vals.push_back(make_pair(make_int(30), make_int(70)));

    auto val = make_stdcpp_hash_table_cont(cont_name, false, cm.bt_int(), cm.bt_int(), vals);
    auto res = ctx.format(val);
    check_simple_map(res, cont_name, L"std::pair");
}

/// Tests formatting simple libc++ map container with specified name
static void test_simple_libcxx_map(context & ctx, const std::string & cont_name) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    std::vector<value> vals;
    vals.push_back(make_pair(make_int(10), make_int(50)));
    vals.push_back(make_pair(make_int(20), make_int(60)));
    vals.push_back(make_pair(make_int(30), make_int(70)));

    auto val = make_libcxx_map(cont_name, cm.bt_int(), cm.bt_int(), vals);
    auto res = ctx.format(val);
    check_simple_map(res, cont_name, L"std::__1::pair");
}

BOOST_AUTO_TEST_CASE(test_stdcpp_map) {
    test_simple_stdcpp_map(ctx, "unordered_map");
}

BOOST_AUTO_TEST_CASE(test_stdcpp_multimap) {
    test_simple_stdcpp_map(ctx, "unordered_multimap");
}

BOOST_AUTO_TEST_CASE(test_libcxx_map) {
    test_simple_libcxx_map(ctx, "unordered_map");
}

BOOST_AUTO_TEST_CASE(test_libcxx_multimap) {
    test_simple_libcxx_map(ctx, "unordered_multimap");
}


/// Checks result of formatting hash table iterator
void check_iterator(const fmt_result_ref & res, const std::wstring & type_name) {
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

/// Tests displaying libstdc++ hash table iterator
BOOST_AUTO_TEST_CASE(test_stdcpp_iterator) {

    auto cm_res = stdcpp_hash_table_cm("unordered_set", true, cm.bt_int(), cm.bt_int());

    cm_builder b;
    auto it_res = b
        .ns("std")
            .ns("__detail")
                .templ("_Node_iterator", "T").record("iterator", cm.bt_int())
                    .ivar("_M_cur", b.ptype(cm_res.type("node")))
                .end().end()
            .end()
        .end()
    .build();

    auto node = make_val(cm_res.type("node"));
    auto i = make_int(10);
    node["_M_storage"]["_M_storage"].cast(i.type()) << i;
    auto it = make_val(it_res.type("iterator"));
    it["_M_cur"] << node.ptr();

    auto res = ctx.format(it);
    check_iterator(res, L"std::__detail::_Node_iterator<int>");
}


cm::type_t * get_libcxx_hash_const_iterator(const cm::qual_type & elt_type) {
    auto & tctx = test_context::current_context();
    auto cm_res = libcxx_hash_table_cm(tctx.cm().bt_int());

    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("__hash_const_iterator", "T").record("iterator", b.ptype(cm_res.type("hash_node")))
                .ivar("__node_", b.ptype(cm_res.type("hash_node_base")))
            .end().end()
        .end().end()
    .build().type("iterator");  
}


/// Tests displaying libc++ hash table iterator
BOOST_AUTO_TEST_CASE(test_libcxx_iterator) {
    auto cm_res = libcxx_hash_table_cm(cm.bt_int());
    auto it_type = get_libcxx_hash_const_iterator(cm.bt_int());

    auto node = make_val(cm_res.type("hash_node"));
    node["__value_"] << make_int(10);
    auto it = make_val(it_type);
    it["__node_"] << node.ptr();

    auto res = ctx.format(it);
    check_iterator(res, L"std::__1::__hash_const_iterator<std::__1::__hash_node<int, void*>*>");
}


/// Tests displaying libc++ hash map iterator
BOOST_AUTO_TEST_CASE(test_libcxx_map_iterator) {

    auto cm_res = libcxx_hash_table_cm(cm.bt_int());
    auto hash_it_type = get_libcxx_hash_const_iterator(cm.bt_int());

    cm_builder b;
    auto it_type = b
        .ns("std").ns("__1")
            .templ("__hash_map_iterator", "T").record("iterator", hash_it_type)
                .ivar("__i_", hash_it_type)
            .end().end()
        .end().end()
    .build().type("iterator");

    auto node = make_val(cm_res.type("hash_node"));
    node["__value_"] << make_int(10);
    auto it = make_val(it_type);
    it["__i_"]["__node_"] << node.ptr();

    auto res = ctx.format(it);
    check_iterator(res, L"std::__1::__hash_const_iterator<std::__1::__hash_node<int, void*>*>");
}



BOOST_AUTO_TEST_SUITE_END()


}
