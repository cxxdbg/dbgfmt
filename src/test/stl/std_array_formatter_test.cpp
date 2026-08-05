
/// \file std_array_formatter_test.cpp
/// Contains unit tests for std::array formatter.

#include "../test2.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Makes libstdc++ std::array record value
static value make_stdcpp_array(cm::type_t * elt_type, const std::vector<value> & vals) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto std = cm.get_or_create_namespace("std");
    auto array = std->get_or_create_template_record("array",
                                              cm::record_kind::class_,
                                              false,
                                              "T",
                                              std::make_tuple("N", cm.bt_int()));

    auto array_inst = array->find_instantiation(elt_type, cm::value{vals.size()});
    if (!array_inst) {
        array_inst = array->create_instantiation(elt_type, cm::value{vals.size()});
        if (!vals.empty()) {
            auto arr_type = cm.get_or_create_arr_type(elt_type, vals.size());
            array_inst->create_field("_M_elems", arr_type);
        } else {
            // _M_elems should be empty struct for zero size array
            auto dummy_rec = std->create_record(cm::record_kind::class_);
            tctx.dbg().make_def_rec_layout(dummy_rec);
            array_inst->create_field("_M_elems", dummy_rec);
        }

        tctx.dbg().make_def_rec_layout(array_inst);
    }

    auto rec = make_val(array_inst);
    if (!vals.empty()) {
        auto arr = make_arr(elt_type, vals.size());
        for (size_t i = 0, e = vals.size(); i < e; ++i) {
            arr[i] << vals[i];
        }

        rec["_M_elems"] << arr;
    }

    return rec;
}


/// Makes CM for testing libc++ array
static auto libcxx_array_cm(const cm::qual_type & elt_type, size_t sz) {
    std::stringstream sz_str;
    sz_str << sz;
    cm::value size_val{sz_str.str()};

    auto real_size = (sz != 0) ? sz : 4;

    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("array", "T", "N").record("array", elt_type, size_val)
                .ivar("__elems_", b.atype(elt_type.type(), real_size))
            .end().end()
        .end().end()
    .build();
}

/// Makes libc++ array
static value make_libcxx_array(const cm::qual_type & elt_type, const std::vector<value> & vals) {
    auto arr_type = libcxx_array_cm(elt_type, vals.size()).type("array");
    auto arr = make_val(arr_type);

    for (size_t i = 0, e = vals.size(); i < e; ++i) {
        arr["__elems_"][i] << vals[i];
    }

    return arr;
}


BOOST_FIXTURE_TEST_SUITE(std_array_formatter_test, fmt_test_fixture)


/// Checks result of array formatting
void check_empty_array(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 0]");
    BOOST_CHECK(res->type() == L"std::array<int, 0>");

    BOOST_REQUIRE(res->childs_size() == 1);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"0");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);
}

/// Tests empty libstdc++ array formatting
BOOST_AUTO_TEST_CASE(empty_stdcpp_array) {
    auto arr = make_stdcpp_array(cm.bt_int(), {});
    auto res = ctx.format(arr);
    check_empty_array(res);
}

/// Tests empty libc++ array formatting
BOOST_AUTO_TEST_CASE(empty_libcxx_array) {
    auto arr = make_libcxx_array(cm.bt_int(), {});
    auto res = ctx.format(arr);
    check_empty_array(res);
}


/// Checks result of simple std::array formatting
void check_simple_array(const fmt_result_ref & res) {
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 2]");
    BOOST_CHECK(res->type() == L"std::array<int, 2>");

    BOOST_REQUIRE(res->childs_size() == 3);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"2");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto item1 = res->child_at(1);
    BOOST_REQUIRE(item1);
    BOOST_CHECK(item1->name() == L"[0]");
    BOOST_CHECK(item1->val() == L"10");
    BOOST_CHECK(item1->type() == L"int");
    BOOST_CHECK(item1->childs_size() == 0);

    auto item2 = res->child_at(2);
    BOOST_REQUIRE(item2);
    BOOST_CHECK(item2->name() == L"[1]");
    BOOST_CHECK(item2->val() == L"20");
    BOOST_CHECK(item2->type() == L"int");
    BOOST_CHECK(item2->childs_size() == 0);
}

/// Tests simple libstdc++ array formatting
BOOST_AUTO_TEST_CASE(simple_stdcpp_array) {
    std::vector<value> vals = {make_int(10), make_int(20)};
    auto arr = make_stdcpp_array(cm.bt_int(), vals);
    auto res = ctx.format(arr);
    check_simple_array(res);
}

/// Tests simple libc++ array formatting
BOOST_AUTO_TEST_CASE(simple_libcxx_array) {
    std::vector<value> vals = {make_int(10), make_int(20)};
    auto arr = make_libcxx_array(cm.bt_int(), vals);
    auto res = ctx.format(arr);
    check_simple_array(res);
}


BOOST_AUTO_TEST_SUITE_END()


}
