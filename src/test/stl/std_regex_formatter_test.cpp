
/// \file std_regex_formatter_test.cpp
/// Contains unit tests for formatters for regex related types.

#include "../test2.hpp"
#include <cm/qual_type.hpp>
#include "../test_context.hpp"
#include "libcxx/libcxx_utils.hpp"
#include "libcxx/libcxx_vector.hpp"
#include "libcxx/libcxx_wrap_iter.hpp"
#include "stdcpp/stdcpp_vector.hpp"
#include "stdcpp/stdcpp_normal_iterator.hpp"
#include "stdcpp/stdcpp_utils.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


namespace tcm::stdcpp::sub_match {
    /// Gets or creates libstdc++ sub_match type
    cm::type_t * type(const cm::qual_type & char_type) {
        auto it_type = tcm::stdcpp::normal_iterator::type(char_type);

        cm_builder b;
        return b
            .ns("std")
                .ns("__cxx11")
                    .templ("sub_match", "Iterator").record("sub_match", it_type)
                        .base(get_stdcpp_pair_type(it_type, it_type))
                    .end().end()
                .end()
            .end()
        .build().type("sub_match");
    }

    /// Creates libstdc++ sub_match value
    value make(const cm::qual_type & elt_type, const value & begin_ptr, const value & end_ptr) {
        auto val = test::make_val(type(elt_type));
        val["first"] << normal_iterator::make(elt_type, begin_ptr);
        val["second"] << normal_iterator::make(elt_type, end_ptr);
        return val;
    }
}


namespace tcm::stdcpp::match_results {
    /// Gets or creates libstdc++ match_results type for specified char type
    cm::type_t * type(const cm::qual_type & char_type) {
        auto it_type = tcm::stdcpp::normal_iterator::type(char_type);

        cm_builder b;
        return b
            .ns("std")
                .ns("__cxx11")
                    .templ("match_results", "Iterator").record("match_results", it_type)
                        .base(get_stdcpp_vector_type(sub_match::type(char_type)))
                    .end().end()
                .end()
            .end()
        .build().type("match_results");
    }

    /// Creates match results value from vector for sub_match values
    value make(const cm::qual_type & char_type, const std::vector<value> & matches) {
        // creating three empty sub_match values to put at the end of vector
        auto vec_vals = matches;
        for (int i = 0; i < 3; ++i) {
            vec_vals.push_back(make_val(sub_match::type(char_type)));
        }

        // creating vector of sub_match values
        auto vec_val = make_stdcpp_vector(sub_match::type(char_type), vec_vals);

        // creating result value
        auto res = make_val(type(char_type));
        res.base_at(0) << vec_val;
        return res;
    }
}


namespace tcm::libcxx::sub_match {
    /// Gets or creates libc++ sub_match type
    cm::type_t * type(const cm::qual_type & char_type) {
        auto it_type = tcm::libcxx::wrap_iter::type(char_type);

        cm_builder b;
        return b
            .ns("std")
                .ns("__1")
                    .templ("sub_match", "Iterator").record("sub_match", it_type)
                        .base(get_libcxx_pair_type(it_type, it_type))
                    .end().end()
                .end()
            .end()
        .build().type("sub_match");
    }

    /// Creates libstdc++ sub_match value
    value make(const cm::qual_type & elt_type, const value & begin_ptr, const value & end_ptr) {
        auto val = test::make_val(type(elt_type));
        val["first"] << wrap_iter::make(elt_type, begin_ptr);
        val["second"] << wrap_iter::make(elt_type, end_ptr);
        return val;
    }
}


namespace tcm::libcxx::match_results {
    /// Gets or creates libc++ match_results type for specified char type
    cm::type_t * type(const cm::qual_type & char_type) {
        auto it_type = tcm::libcxx::wrap_iter::type(char_type);

        cm_builder b;
        return b
            .ns("std")
                .ns("__1")
                    .templ("match_results", "Iterator").record("match_results", it_type)
                        .ivar("__matches_", libcxx::vector::type(sub_match::type(char_type)))
                    .end().end()
                .end()
            .end()
        .build().type("match_results");
    }

    /// Creates match results value from vector for sub_match values
    value make(const cm::qual_type & char_type, const std::vector<value> & matches) {
        auto res = make_val(type(char_type));
        res["__matches_"] << vector::make(sub_match::type(char_type), matches);
        return res;
    }
}


BOOST_FIXTURE_TEST_SUITE(stdcpp_vector_formatter, fmt_test_fixture)


/// Tests formatting std sub_match value
static void test_sub_match(context & ctx, bool is_libcxx) {
    auto & cm = test_context::current_context().cm();

    std::string_view str{"Simple string"};
    auto arr = make_char_arr(str);
    auto val = is_libcxx ?
               tcm::libcxx::sub_match::make(cm.bt_char(), arr[0].ptr() + 4, arr[0].ptr() + 10) :
               tcm::stdcpp::sub_match::make(cm.bt_char(), arr[0].ptr() + 4, arr[0].ptr() + 10);

    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"\"le str\"");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 9);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"6");

    auto begin = res->child_at(1);
    BOOST_REQUIRE(begin);
    BOOST_CHECK(begin->name() == L"[begin]");
    BOOST_CHECK(begin->val() == L"'l' (108)");

    auto end = res->child_at(2);
    BOOST_REQUIRE(end);
    BOOST_CHECK(end->name() == L"[end]");
    BOOST_CHECK(end->val() == L"'i' (105)");

    auto ch0 = res->child_at(3);
    BOOST_REQUIRE(ch0);
    BOOST_CHECK(ch0->name() == L"[0]");
    BOOST_CHECK(ch0->val() == L"'l' (108)");

    auto ch1 = res->child_at(4);
    BOOST_REQUIRE(ch1);
    BOOST_CHECK(ch1->name() == L"[1]");
    BOOST_CHECK(ch1->val() == L"'e' (101)");

    auto ch2 = res->child_at(5);
    BOOST_REQUIRE(ch2);
    BOOST_CHECK(ch2->name() == L"[2]");
    BOOST_CHECK(ch2->val() == L"' ' (32)");

    auto ch3 = res->child_at(6);
    BOOST_REQUIRE(ch3);
    BOOST_CHECK(ch3->name() == L"[3]");
    BOOST_CHECK(ch3->val() == L"'s' (115)");

    auto ch4 = res->child_at(7);
    BOOST_REQUIRE(ch4);
    BOOST_CHECK(ch4->name() == L"[4]");
    BOOST_CHECK(ch4->val() == L"'t' (116)");

    auto ch5 = res->child_at(8);
    BOOST_REQUIRE(ch5);
    BOOST_CHECK(ch5->name() == L"[5]");
    BOOST_CHECK(ch5->val() == L"'r' (114)");
}

/// Tests formatting libstdc++ sub_match value
BOOST_AUTO_TEST_CASE(test_sub_match_stdcpp) {
    test_sub_match(ctx, false);
}

/// Tests formatting libc++ sub_match value
BOOST_AUTO_TEST_CASE(test_sub_match_libcxx) {
    test_sub_match(ctx, true);
}


/// Tests formatting match_results value
static void test_match_results(context & ctx, bool is_libcxx) {
    auto & cm = test_context::current_context().cm();

    std::string_view str{"Simple string"};
    auto arr = make_char_arr(str);

    // "imple str"
    auto val1 = is_libcxx ?
                tcm::libcxx::sub_match::make(cm.bt_char(), arr[0].ptr() + 1, arr[0].ptr() + 10) :
                tcm::stdcpp::sub_match::make(cm.bt_char(), arr[0].ptr() + 1, arr[0].ptr() + 10);

    // "le str"
    auto val2 = is_libcxx ?
                tcm::libcxx::sub_match::make(cm.bt_char(), arr[0].ptr() + 4, arr[0].ptr() + 10) :
                tcm::stdcpp::sub_match::make(cm.bt_char(), arr[0].ptr() + 4, arr[0].ptr() + 10);

    // "ing"
    auto val3 = is_libcxx ?
                tcm::libcxx::sub_match::make(cm.bt_char(), arr[0].ptr() + 10, arr[0].ptr() + 13) :
                tcm::stdcpp::sub_match::make(cm.bt_char(), arr[0].ptr() + 10, arr[0].ptr() + 13);

    auto val = is_libcxx ?
               tcm::libcxx::match_results::make(cm.bt_char(), {val1, val2, val3}) :
               tcm::stdcpp::match_results::make(cm.bt_char(), {val1, val2, val3});

    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"[size = 3]");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 4);

    auto child0 = res->child_at(0);
    BOOST_REQUIRE(child0);
    BOOST_CHECK(child0->name() == L"[size]");
    BOOST_CHECK(child0->val() == L"3");

    auto child1 = res->child_at(1);
    BOOST_REQUIRE(child1);
    BOOST_CHECK(child1->name() == L"[0]");
    BOOST_CHECK(child1->val() == L"\"imple str\"");

    auto child2 = res->child_at(2);
    BOOST_REQUIRE(child2);
    BOOST_CHECK(child2->name() == L"[1]");
    BOOST_CHECK(child2->val() == L"\"le str\"");

    auto child3 = res->child_at(3);
    BOOST_REQUIRE(child3);
    BOOST_CHECK(child3->name() == L"[2]");
    BOOST_CHECK(child3->val() == L"\"ing\"");
}

BOOST_AUTO_TEST_CASE(test_match_results_stdcpp) {
    test_match_results(ctx, false);
}

BOOST_AUTO_TEST_CASE(test_match_results_libcxx) {
    test_match_results(ctx, true);
}



BOOST_AUTO_TEST_SUITE_END()


}
