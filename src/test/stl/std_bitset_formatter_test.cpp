
/// \file std_bitset_formatter_test.cpp
/// Contains unit tests for std::bitset formatter

#include "../test2.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


namespace {
    enum class bitset_kind {
        stdcpp,
        libcxx
    };
}


/// Calculates size of unsigned long array in bitset for specified size of bitset
static size_t calc_bitset_size(size_t sz) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    // calculating size of unsigned long array in bitset
    auto long_sz = tctx.dbg().type_size(cm.bt_unsigned_long()) * 8;
    auto bitset_sz = sz / long_sz;
    if (sz % long_sz != 0) {
        ++bitset_sz;
    }

    return bitset_sz;
}


/// Returns data type for bitset of specified size
cm::type_t * get_bitset_data_type(size_t sz) {
    auto & cm = test_context::current_context().cm();
    auto bitset_sz = calc_bitset_size(sz);

    if (bitset_sz == 1) {
        return cm.bt_unsigned_long();
    } else {
        return cm.get_or_create_arr_type(cm.bt_unsigned_long(), bitset_sz);
    }
}


/// Gets or creates libstdc++ bitset type
cm::record_type * get_stdcpp_bitset_type(size_t sz) {
    cm_builder b;
    return b
        .ns("std")
            .templ("_Base_bitset", "N").record("base", cm::value{sz})
                .ivar("_M_w", get_bitset_data_type(sz))
            .end().end()

            .templ("bitset", "N").record("bitset", cm::value{sz})
                .base(b.typeref("base"))
            .end().end()
        .end()
    .build().rtype("bitset");
}


/// Gets or creates libc++ bitset type
cm::record_type * get_libcxx_bitset_type(size_t sz) {
    cm_builder b;
    return b
        .ns("std").ns("__1")
            .templ("__bitset", "N").record("base", cm::value{sz})
                .ivar("__first_", get_bitset_data_type(sz))
            .end().end()

            .templ("bitset", "N").record("bitset", cm::value{sz})
                .base(b.typeref("base"))
            .end().end()
        .end().end()
    .build().rtype("bitset");
}


/// Creates bitset of specified type with specified array of values
static value create_bitset(const cm::qual_type & bitset_type,
                           const std::string & data_fld_name,
                           const std::vector<bool> & vals) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto bitset_sz = calc_bitset_size(vals.size());
    auto long_sz = tctx.dbg().type_size(cm.bt_unsigned_long()) * 8;

    auto bs = make_val(bitset_type);
    for (size_t i = 0; i < vals.size(); ++i) {
        auto arr_idx = i / long_sz;
        auto bit_idx = i % long_sz;

        auto long_fld = (bitset_sz == 1) ? bs[data_fld_name] : bs[data_fld_name][arr_idx];

        // clearing array element if processing first bit
        if (bit_idx == 0) {
            long_fld << 0;
        }

        auto arr_val = long_fld.as_uint64();
        uint64_t bit_val = vals[i] ? 1 : 0;
        bit_val <<= bit_idx;
        arr_val |= bit_val;
        long_fld << arr_val;
    }

    return bs;
}


/// Creates bitset with specified array of values
static value create_bitset(const std::vector<bool> & vals, bitset_kind bs_kind) {
    if (bs_kind == bitset_kind::stdcpp) {
        return create_bitset(get_stdcpp_bitset_type(vals.size()), "_M_w", vals);
    } else {
        return create_bitset(get_libcxx_bitset_type(vals.size()), "__first_", vals);
    }
}


BOOST_FIXTURE_TEST_SUITE(std_bitset_formatter_test, fmt_test_fixture)


/// Tests formatting empty bitset
void test_empty_bitset(context & ctx, bitset_kind bs_kind) {
    auto bs = create_bitset({}, bitset_kind::stdcpp);
    auto res = ctx.format(bs);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 0]");
    BOOST_CHECK(res->type() == L"std::bitset<0>");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 1);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"0");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK(sz->childs_size() == 0);
}

/// Tests formatting empty libstdc++ bitset
BOOST_AUTO_TEST_CASE(empty_stdcpp_bitset) {
    test_empty_bitset(ctx, bitset_kind::stdcpp);
}

/// Tests formatting empty libc++ bitset
BOOST_AUTO_TEST_CASE(empty_libcxx_bitset) {
    test_empty_bitset(ctx, bitset_kind::libcxx);
}


/// Tests formatting short bitset
void test_short_bitset(context & ctx, bitset_kind bs_kind) {
    std::vector<bool> vals;
    for (int i = 0; i < 10; ++i) {
        vals.push_back(false);
    }

    vals.push_back(true);

    for (int i = 0; i < 3; ++i) {
        vals.push_back(false);
    }

    vals.push_back(true);

    for (int i = 0; i < 2; ++i) {
        vals.push_back(false);
    }

    auto bs = create_bitset(vals, bs_kind);
    auto res = ctx.format(bs);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 17]");
    BOOST_CHECK(res->type() == L"std::bitset<17>");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 18);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"17");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK(sz->childs_size() == 0);

    for (size_t i = 1; i <= 17; ++i) {
        auto child = res->child_at(i);
        BOOST_REQUIRE(child);

        std::wostringstream num_str;
        num_str << "[" << (i - 1) << "]";

        BOOST_CHECK(child->name() == num_str.str());
        BOOST_CHECK(child->type() == L"bool");
        BOOST_CHECK_EQUAL(child->childs_size(), 0);

        if (i == 11 || i == 15) {
            BOOST_CHECK(child->val() == L"true");
        } else {
            BOOST_CHECK(child->val() == L"false");
        }
    }
}

/// Tests formatting short libstdc++ bitset
BOOST_AUTO_TEST_CASE(short_stdcpp_bitset) {
    test_short_bitset(ctx, bitset_kind::stdcpp);
}

/// Tests formatting short libc++ bitset
BOOST_AUTO_TEST_CASE(short_libcxx_bitset) {
    test_short_bitset(ctx, bitset_kind::libcxx);
}


/// Tests formatting long bitset
void test_long_bitset(context & ctx, bitset_kind bs_kind) {
    std::vector<bool> vals;
    for (int i = 0; i < 35; ++i) {
        vals.push_back(false);
    }

    vals.push_back(true);

    for (int i = 0; i < 29; ++i) {
        vals.push_back(false);
    }

    vals.push_back(true);

    auto bs = create_bitset(vals, bitset_kind::stdcpp);
    auto res = ctx.format(bs);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 66]");
    BOOST_CHECK(res->type() == L"std::bitset<66>");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 67);

    auto sz = res->child_at(0);
    BOOST_REQUIRE(sz);
    BOOST_CHECK(sz->name() == L"[size]");
    BOOST_CHECK(sz->val() == L"66");
    BOOST_CHECK(sz->type() == L"std::size_t");
    BOOST_CHECK(sz->childs_size() == 0);

    for (size_t i = 1; i <= 66; ++i) {
        auto child = res->child_at(i);
        BOOST_REQUIRE(child);

        std::wostringstream num_str;
        num_str << "[" << (i - 1) << "]";

        BOOST_CHECK(child->name() == num_str.str());
        BOOST_CHECK(child->type() == L"bool");
        BOOST_CHECK_EQUAL(child->childs_size(), 0);

        if (i == 36 || i == 66) {
            BOOST_CHECK(child->val() == L"true");
        } else {
            BOOST_CHECK(child->val() == L"false");
        }
    }
}

/// Tests displaying long libstdc++ bitset
BOOST_AUTO_TEST_CASE(long_stdcpp_bitset) {
    test_long_bitset(ctx, bitset_kind::stdcpp);
}

/// Tests displaying long libc++ bitset
BOOST_AUTO_TEST_CASE(long_libcxx_bitset) {
    test_long_bitset(ctx, bitset_kind::libcxx);
}


BOOST_AUTO_TEST_SUITE_END()


}
