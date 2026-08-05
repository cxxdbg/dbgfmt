
#include "libcxx/libcxx_utils.hpp"
#include "stdcpp/stdcpp_utils.hpp"
#include "../test2.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


namespace {

enum class list_kind {
    stdcpp,
    stdcpp_cxx11,
    libcxx
};

}


/// Creates code model for testing std::list. Returns builder result.
static cm::builder_result std_list_cm(const cm::qual_type & elt_type, list_kind knd) {
    auto list_node_ivar_name = knd == list_kind::stdcpp_cxx11 ? "_M_storage" : "_M_data";
    auto list_node_ivar_type = knd == list_kind::stdcpp_cxx11 ?
                                      get_stdcpp_aligned_membuf_type(elt_type) :
                                      elt_type;

    cm_builder b;
    return b
        .ns("std")
            .record("_List_node_base", "stdcpp_node_base")
                .ivar("_M_prev", b.ptype(b.typeref("stdcpp_node_base")))
                .ivar("_M_next", b.ptype(b.typeref("stdcpp_node_base")))
            .end()

            .templ("_List_node", "T").record("stdcpp_node", elt_type)
                .base(b.typeref("stdcpp_node_base"))
                .ivar(list_node_ivar_name, list_node_ivar_type)
            .end().end()

            .templ("_List_base", "T").record("stdcpp_list_base", elt_type)
                .record("_List_impl", "stdcpp_impl")
                    .ivar("_M_node", b.typeref("stdcpp_node_base"))
                .end()
            .end().end()

            .templ("list", "T").record("stdcpp_list", elt_type)
                .ivar("_M_impl", b.typeref("stdcpp_impl"))
            .end().end()

            .ns("__cxx11")
                .templ("_List_base", "T").record("stdcpp_list_base_cxx11", elt_type)
                    .record("_List_impl", "stdcpp_impl_cxx11")
                        .ivar("_M_node", b.typeref("stdcpp_node_base"))
                    .end()
                .end().end()

                .templ("list", "T").record("stdcpp_list_cxx11", elt_type)
                    .ivar("_M_impl", b.typeref("stdcpp_impl_cxx11"))
                .end().end()
            .end()

            .templ("_List_iterator", "T").record("stdcpp_iterator", elt_type)
                .ivar("_M_node", b.ptype(b.typeref("stdcpp_node")))
            .end().end()


            // libc++ std::list types
            .ns("__1")
                .templ("__list_node_base", "T").record("libcxx_node_base", elt_type)
                    .ivar("__prev_", b.ptype(b.typeref("libcxx_node_base")))
                    .ivar("__next_", b.ptype(b.typeref("libcxx_node_base")))
                .end().end()

                .templ("__list_impl", "T").record("libcxx_impl", elt_type)
                    .ivar("__end_", b.typeref("libcxx_node_base"))
                    .ivar("__size_alloc_", get_libcxx_compressed_pair_type(b.bt_unsigned_long()))
                .end().end()

                .templ("list", "T").record("libcxx_list", elt_type)
                    .base(b.typeref("libcxx_impl"))
                .end().end()

                .templ("__list_node", "T", "Ptr").record("libcxx_node", elt_type, b.ptype(b.bt_void()))
                    .base(b.typeref("libcxx_node_base"))
                    .ivar("__value_", elt_type)
                .end().end()

                .templ("__list_iterator", "T", "Ptr").record("libcxx_iterator", elt_type, b.ptype(b.bt_void()))
                    .ivar("__ptr_", b.ptype(b.typeref("libcxx_node_base")))
                .end().end()
            .end()
        .end()
    .build();
}


/// Gets or creates type for list node base class
static cm::record_type * get_list_node_base_type(const cm::qual_type & elt_type, list_kind knd) {
    auto cm_res = std_list_cm(elt_type, knd);
    if (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) {
        return cm_res.rtype("stdcpp_node_base");
    } else {
        return cm_res.rtype("libcxx_node_base");
    }
}


/// Gets or creates type for list node class
static cm::record_type * get_list_node_type(const cm::qual_type & elt_type, list_kind knd) {
    auto cm_res = std_list_cm(elt_type, knd);

    if (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) {
        return cm_res.rtype("stdcpp_node");
    } else {
        return cm_res.rtype("libcxx_node");
    }
}


/// Gets or creates type for list
static cm::record_type * get_list_type(const cm::qual_type & elt_type, list_kind knd) {
    auto res = std_list_cm(elt_type, knd);
    if (knd == list_kind::stdcpp) {
        return res.rtype("stdcpp_list");
    } else if (knd == list_kind::stdcpp_cxx11) {
        return res.rtype("stdcpp_list_cxx11");
    } else {
        return res.rtype("libcxx_list");
    }
}


/// Gets or creates type for list iterator
static cm::record_type * get_list_iterator_type(const cm::qual_type & elt_type, list_kind knd) {
    auto cm_res = std_list_cm(elt_type, knd);
    if (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) {
        return cm_res.rtype("stdcpp_iterator");
    } else {
        return cm_res.rtype("libcxx_iterator");
    }
}


/// Creates C++11 list node for specifeid value
static value create_list_node(const value & val, list_kind knd) {
    auto node_type = get_list_node_type(val_type(val), knd);
    auto rec = make_val(node_type);

    if (knd == list_kind::stdcpp_cxx11) {
        const auto & cdata = rec["_M_storage"]["_M_storage"];
        cdata.cast(val.type()) << val;
    } else if (knd == list_kind::stdcpp) {
        rec["_M_data"] << val;
    } else {
        rec["__value_"] << val;
    }

    return rec;
}


/// Creates C++11 list with specified vector of values
static value create_list(const cm::qual_type & elt_type, const std::vector<value> & vals, list_kind knd) {                    

    // creating list record
    auto list_type = get_list_type(elt_type, knd);
    auto list = make_val(list_type);
    auto root_node = (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) ?
                     list["_M_impl"]["_M_node"] :
                     list["__end_"];

    auto prev_name = (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) ? "_M_prev" : "__prev_";
    auto next_name = (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) ? "_M_next" : "__next_";

    auto node_base_type = get_list_node_base_type(elt_type, knd);

    root_node[prev_name] << root_node.ptr();
    root_node[next_name] << root_node.ptr();

    // creating list nodes

    value last_node;

    for (auto && val : vals) {
        auto node = create_list_node(val, knd);
        if (last_node.is_valid()) {
            last_node[next_name] << node.ptr();
            node[prev_name] << last_node.ptr();
        } else {
            root_node[next_name] << node.ptr();
            node[prev_name] << root_node.ptr();
        }

        last_node = node;
    }

    if (last_node.is_valid()) {
        last_node[next_name] << root_node.ptr();
        root_node[prev_name] << last_node.ptr();
    }

    return list;
}


/// Returns iterator pointing to the first element of the C++11 list
static value create_list_iterator(const value & val, list_kind knd) {
    // creating list node
    auto node = create_list_node(val, knd);

    // creating iterator
    auto type = get_list_iterator_type(val_type(val), knd);
    auto rec = make_val(type);

    if (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) {
        rec["_M_node"] << node.ptr();
    } else {
        rec["__ptr_"] << node.ptr();
    }
    return rec;
}


std::wstring get_list_tname(list_kind knd) {
    if (knd == list_kind::stdcpp) {
        return L"std::list";
    } else if (knd == list_kind::stdcpp_cxx11) {
        return L"std::__cxx11::list";
    } else {
        return L"std::list";
    }
}


/// Tests formatting of empty list
static void test_empty_list(context & ctx, cm::code_model & cm, list_kind knd) {
    auto list = create_list(cm.bt_int(), {}, knd);
    auto res = ctx.format(list);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 0]");
    BOOST_CHECK(res->type() == get_list_tname(knd) + L"<int>");
    BOOST_REQUIRE(res->childs_size() == 1);

    auto size_child = res->child_at(0);
    BOOST_REQUIRE(size_child);
    BOOST_CHECK(size_child->name() == L"[size]");
    BOOST_CHECK(size_child->val() == L"0");
    BOOST_CHECK(size_child->childs_size() == 0);
}


/// Tests formatting of simple list
static void test_simple_list(context & ctx, cm::code_model & cm, list_kind knd) {
    std::vector<value> vals = {make_int(10), make_int(20)};
    auto res = ctx.format(create_list(cm.bt_int(), vals, knd));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 2]");
    BOOST_CHECK(res->type() == get_list_tname(knd) + L"<int>");
    BOOST_REQUIRE(res->childs_size() == 3);

    auto size_child = res->child_at(0);
    BOOST_REQUIRE(size_child);
    BOOST_CHECK(size_child->name() == L"[size]");
    BOOST_CHECK(size_child->val() == L"2");
    BOOST_CHECK(size_child->childs_size() == 0);

    auto val1 = res->child_at(1);
    BOOST_REQUIRE(val1);
    BOOST_CHECK(val1->name() == L"[0]");
    BOOST_CHECK(val1->val() == L"10");
    BOOST_CHECK(val1->type() == L"int");

    auto val2 = res->child_at(2);
    BOOST_REQUIRE(val2);
    BOOST_CHECK(val2->name() == L"[1]");
    BOOST_CHECK(val2->val() == L"20");
    BOOST_CHECK(val2->type() == L"int");
}


/// Tests formatting of list iterator
static void test_iterator(context & ctx, cm::code_model & cm, list_kind knd) {
    auto it = create_list_iterator(make_int(10), knd);

    // address of value has 16 byte offset from start of node
    auto * fld_name = (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) ? "_M_node" : "__ptr_";
    auto addr = it[fld_name].as_uint64() + 16;

    std::wstring tname = (knd == list_kind::stdcpp || knd == list_kind::stdcpp_cxx11) ?
                         L"std::_List_iterator<int>" :
                         L"std::__1::__list_iterator<int, void*>";

    auto res = ctx.format(it);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10");
    BOOST_CHECK(res->type() == tname);

    BOOST_REQUIRE(res->childs_size() == 1);
    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"[ptr]");
    BOOST_CHECK(child->val() == addr_str(addr) + L" 10");
    BOOST_CHECK(child->childs_size() == 1);

    auto child_child = child->child_at(0);
    BOOST_REQUIRE(child_child);
    BOOST_CHECK(child_child->name() == L"");
    BOOST_CHECK(child_child->val() == L"10");
    BOOST_CHECK(child_child->type() == L"int");
    BOOST_CHECK(child_child->childs_size() == 0);
}


/// Tests displaying list of template instancies (space in double > >)
static void test_list_template(context & ctx,
                               cm::code_model & cm,
                               cm::debug_info & dbg,
                               list_kind knd) {

    auto cls = cm.get_or_create_template_record("cls", cm::record_kind::class_, false, "T");
    auto cls_inst = cls->create_instantiation(cm.bt_int());
    cls_inst->create_field("fld", cm.bt_int());
    dbg.make_def_rec_layout(cls_inst);

    std::vector<value> vals;
    auto val = make_val(cls_inst);
    val["fld"] << 10;
    vals.push_back(val);

    auto res = ctx.format(create_list(cls_inst, vals, knd));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 1]");
    BOOST_CHECK(res->type() == get_list_tname(knd) + L"<cls<int> >");
    BOOST_REQUIRE(res->childs_size() == 2);

    auto size_child = res->child_at(0);
    BOOST_REQUIRE(size_child);
    BOOST_CHECK(size_child->name() == L"[size]");
    BOOST_CHECK(size_child->val() == L"1");
    BOOST_CHECK(size_child->childs_size() == 0);

    auto val1 = res->child_at(1);
    BOOST_REQUIRE(val1);
    BOOST_CHECK(val1->name() == L"[0]");
    BOOST_CHECK(val1->val() == L"{ fld=10 }");
    BOOST_CHECK(val1->type() == L"cls<int>");
    BOOST_CHECK(val1->childs_size() == 1);
}


BOOST_FIXTURE_TEST_SUITE(std_list_formatter, fmt_test_fixture)


BOOST_AUTO_TEST_CASE(empty_list_stdcpp_cxx11) {
    test_empty_list(ctx, cm, list_kind::stdcpp_cxx11);
}

BOOST_AUTO_TEST_CASE(simple_list_stdcpp_cxx11) {
    test_simple_list(ctx, cm, list_kind::stdcpp_cxx11);
}

BOOST_AUTO_TEST_CASE(iterator_test_stdcpp_cxx11) {
    test_iterator(ctx, cm, list_kind::stdcpp_cxx11);
}

BOOST_AUTO_TEST_CASE(list_template_stdcpp_cxx11) {
    test_list_template(ctx, cm, dbg, list_kind::stdcpp_cxx11);
}


BOOST_AUTO_TEST_CASE(empty_list_stdcpp) {
    test_empty_list(ctx, cm, list_kind::stdcpp);
}

BOOST_AUTO_TEST_CASE(simple_list_stdcpp) {
    test_simple_list(ctx, cm, list_kind::stdcpp);
}

BOOST_AUTO_TEST_CASE(iterator_test_stdcpp) {
    test_iterator(ctx, cm, list_kind::stdcpp);
}

BOOST_AUTO_TEST_CASE(list_template_stdcpp) {
    test_list_template(ctx, cm, dbg, list_kind::stdcpp);
}


BOOST_AUTO_TEST_CASE(empty_list_libcxx) {
    test_empty_list(ctx, cm, list_kind::libcxx);
}

BOOST_AUTO_TEST_CASE(simple_list_libcxx) {
    test_simple_list(ctx, cm, list_kind::libcxx);
}

BOOST_AUTO_TEST_CASE(iterator_test_libcxx) {
    test_iterator(ctx, cm, list_kind::libcxx);
}

BOOST_AUTO_TEST_CASE(list_template_libcxx) {
    test_list_template(ctx, cm, dbg, list_kind::libcxx);
}


BOOST_AUTO_TEST_SUITE_END()


}
