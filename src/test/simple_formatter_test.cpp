
/// \file simple_formatter.cpp
/// Contains unit tests for simple formatter.

#include "array_value.hpp"
#include "builtin_value.hpp"
#include "cm_builder.hpp"
#include "enum_value.hpp"
#include "error_value.hpp"
#include "fmt_test_fixture.hpp"
#include "pointer_value.hpp"
#include "record_value.hpp"
#include "dbgfmt/format_options.hpp"
#include "util.hpp"
#include "dbgfmt/fmt_result.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(simple_formatter, fmt_test_fixture)


BOOST_AUTO_TEST_CASE(bool_value_true_test) {
    auto res = ctx.format(bool_value(true));
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"bool");
    BOOST_CHECK(res->val() == L"true");
}


BOOST_AUTO_TEST_CASE(bool_value_false_test) {
    auto res = ctx.format(bool_value(false));
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"bool");
    BOOST_CHECK(res->val() == L"false");
}


/// Tests displaying corrupted boolean value
BOOST_AUTO_TEST_CASE(bool_value_corrupted_test) {
    auto res = ctx.format(bool_value(37));
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"bool");
    BOOST_CHECK(res->val() == L"37");
}


/// Tests displaying corrupted boolean value in hex mode
BOOST_AUTO_TEST_CASE(bool_value_corrupted_test_hex) {
    ctx.set_hex(true);
    auto res = ctx.format(bool_value(37));
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"bool");
    BOOST_CHECK(res->val() == L"0x25");
}


/// Tests reading bool value with error
BOOST_AUTO_TEST_CASE(bool_value_error_test) {
    auto res = ctx.format(error_value(cm.bt_bool()));
    BOOST_CHECK(is_error_res(res, L"bool"));
}


/// Tests formatting int value
BOOST_AUTO_TEST_CASE(int_value_test) {
    auto res = ctx.format(int_value(10));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10");
    BOOST_CHECK(res->type() == L"int");
}


/// Tests formatting int value in hex mode
BOOST_AUTO_TEST_CASE(int_value_test_hex) {
    ctx.set_hex(true);
    auto res = ctx.format(int_value(10));

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x0000000A");
    BOOST_CHECK(res->type() == L"int");
}


/// Tests reading int value with error
BOOST_AUTO_TEST_CASE(int_value_error_test) {
    auto res = ctx.format(error_value(cm.bt_int()));
    BOOST_CHECK(is_error_res(res, L"int"));
}


BOOST_AUTO_TEST_CASE(float_value_test) {
    auto res = ctx.format(float_value{10.2f});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10.2");
    BOOST_CHECK(res->type() == L"float");
}


/// Tests reading float value with error
BOOST_AUTO_TEST_CASE(float_value_error_test) {
    auto res = ctx.format(error_value{cm.bt_float()});
    BOOST_CHECK(is_error_res(res, L"float"));
}


/// Tests formatting character
BOOST_AUTO_TEST_CASE(char_value_test) {
    auto res = ctx.format(char_value{'t'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"char");
    BOOST_CHECK(res->val() == L"'t' (116)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting character in hex mode
BOOST_AUTO_TEST_CASE(char_value_test_hex) {
    ctx.set_hex(true);
    auto res = ctx.format(char_value{'t'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"char");
    BOOST_CHECK(res->val() == L"'t' (0x74)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting negative character
BOOST_AUTO_TEST_CASE(neg_char_value_test) {
    auto res = ctx.format(char_value{-1});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"char");
    BOOST_CHECK(res->val() == L"'\\xFF' (-1)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting negative character in hex mode
BOOST_AUTO_TEST_CASE(neg_char_value_test_hex) {
    ctx.set_hex(true);
    auto res = ctx.format(char_value{-1});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"char");
    BOOST_CHECK(res->val() == L"'\\xFF' (0xFF)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests displaying \r character
BOOST_AUTO_TEST_CASE(char_10_value_test) {
    auto res = ctx.format(char_value{'\r'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"char");
    BOOST_CHECK(res->val() == L"'\\r' (13)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests displaying \r character in hex mode
BOOST_AUTO_TEST_CASE(char_10_value_test_hex) {
    ctx.set_hex(true);
    auto res = ctx.format(char_value{'\r'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"char");
    BOOST_CHECK(res->val() == L"'\\r' (0x0D)");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting wide character
BOOST_AUTO_TEST_CASE(wchar_value_test) {
    auto res = ctx.format(wchar_value{L'x'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"wchar_t");
    BOOST_CHECK(res->val() == L"'x' (120)");
}


/// Tests formatting wide character in hex mode
BOOST_AUTO_TEST_CASE(wchar_value_test_hex) {
    ctx.set_hex(true);
    auto res = ctx.format(wchar_value{L'x'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"wchar_t");
    BOOST_CHECK(res->val() == L"'x' (0x78)");
}


/// Tests reading wchar value with error
BOOST_AUTO_TEST_CASE(wchar_value_error_test) {
    auto res = ctx.format(error_value{cm.bt_wchar_t()});
    BOOST_CHECK(is_error_res(res, L"wchar_t"));
}


/// Tests displaying typedef value
BOOST_AUTO_TEST_CASE(typedef_value_test) {

    // creating typedef for int type in code model
    auto td = cm.create_typedef("my_int", cm.bt_int());

    int_value i{10};
    auto val = i.cast(td);
    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10");
    BOOST_CHECK(res->type() == L"int");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting enum value
BOOST_AUTO_TEST_CASE(enum_value_test) {
    auto en_type = cm.create_enum("my_enum", cm.bt_short());
    en_type->items().push_back({"my_item_1", 0});
    en_type->items().push_back({"my_item_2", 1});
    en_type->items().push_back({"my_item_3", 2});

    auto res = ctx.format(enum_value{en_type, 1});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"my_item_2 (1)");
    BOOST_CHECK(res->type() == L"my_enum");
}


/// Tests formatting enum value in hex mode
BOOST_AUTO_TEST_CASE(enum_value_test_hex) {
    auto en_type = cm.create_enum("my_enum", cm.bt_short());
    en_type->items().push_back({"my_item_1", 0});
    en_type->items().push_back({"my_item_2", 1});
    en_type->items().push_back({"my_item_3", 2});

    ctx.set_hex(true);
    auto res = ctx.format(enum_value{en_type, 1});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"my_item_2 (0x0001)");
    BOOST_CHECK(res->type() == L"my_enum");
}


/// Tests formatting invalid enum value
BOOST_AUTO_TEST_CASE(enum_value_invalid_test) {
    auto en_type = cm.create_enum("my_enum", cm.bt_short());
    en_type->items().push_back({"my_item_1", 0});
    en_type->items().push_back({"my_item_2", 1});
    en_type->items().push_back({"my_item_3", 2});

    auto res = ctx.format(enum_value{en_type, 5});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"5");
    BOOST_CHECK(res->type() == L"my_enum");
}


/// Tests formatting invalid enum value in hex mode
BOOST_AUTO_TEST_CASE(enum_value_invalid_test_hex) {
    auto en_type = cm.create_enum("my_enum", cm.bt_short());
    en_type->items().push_back({"my_item_1", 0});
    en_type->items().push_back({"my_item_2", 1});
    en_type->items().push_back({"my_item_3", 2});

    ctx.set_hex(true);
    auto res = ctx.format(enum_value{en_type, 5});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x0005");
    BOOST_CHECK(res->type() == L"my_enum");
}


/// Tests displaying enum value with error
BOOST_AUTO_TEST_CASE(enum_value_error_test) {
    auto en_type = cm.create_enum("my_enum", cm.bt_short());
    en_type->items().push_back({"my_item_1", 0});
    en_type->items().push_back({"my_item_2", 1});
    en_type->items().push_back({"my_item_3", 2});

    auto res = ctx.format(error_value{en_type});
    BOOST_CHECK(is_error_res(res, L"my_enum"));
}


/// Tests error in displaing value child
BOOST_AUTO_TEST_CASE(child_error_test) {
    auto rtype = cm.create_named_record("my_record");
    rtype->create_field("fld", cm.bt_int());
    dbg.make_def_rec_layout(rtype);

    error_value rec{rtype};
    auto res = ctx.format(rec);

    BOOST_REQUIRE(res);

    BOOST_REQUIRE(res->val().size() > 6 + 2);
    BOOST_CHECK(starts_with(res->val(), L"{ fld="));
    BOOST_CHECK(is_error_msg(res->val().substr(6, res->val().size() - 6 - 2)));
    BOOST_CHECK(res->val().substr(res->val().size() - 2) == L" }");

    BOOST_CHECK(res->type() == L"my_record");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto child = res->child_at(0);

    BOOST_CHECK(is_error_res(child, L"int", L"fld"));
}


/// Tests displaying float complex value
BOOST_AUTO_TEST_CASE(float_complex_test) {
    complex_double_value val(10.1, -20.2);
    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10.1 - 20.2i");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting int complex value
BOOST_AUTO_TEST_CASE(int_complex_test) {
    complex_int_value val(10, -20);
    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10 - 20i");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting int complex value in hex mode
BOOST_AUTO_TEST_CASE(int_complex_test_hex) {
    complex_int_value val(10, -20);
    ctx.set_hex(true);
    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x0000000A - 0x00000014i");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting unsigned int complex value
BOOST_AUTO_TEST_CASE(uint_complex_test) {
    complex_unsigned_int_value val(10, 20);
    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"10 + 20i");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests formatting unsigned int complex value in hex mode
BOOST_AUTO_TEST_CASE(uint_complex_test_hex) {
    complex_unsigned_int_value val(10, 20);
    ctx.set_hex(true);
    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"0x0000000A + 0x00000014i");
    BOOST_CHECK(res->childs_size() == 0);
}


/// Tests printing hex value with specified min hex size
BOOST_AUTO_TEST_CASE(hex_min_size) {
    try {
        ctx.set_min_hex_size(4);
        ctx.set_hex(true);
        auto res = ctx.format(long_value(10));

        BOOST_REQUIRE(res);
        BOOST_CHECK(res->val() == L"0x0000000A");
        BOOST_CHECK(res->type() == L"long");

        ctx.set_min_hex_size(8);
    }
    catch(...) {
        ctx.set_min_hex_size(8);
    }
}


/// Tests formatting unicode wide character
BOOST_AUTO_TEST_CASE(wchar_unicode) {
    auto res = ctx.format(wchar_value{L'ж'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"wchar_t");
    BOOST_CHECK(res->val() == L"'ж' (1078)");
}


/// Tests formatting unicode wide character in hex mode
BOOST_AUTO_TEST_CASE(wchar_unicode_hex) {
    ctx.set_hex(true);
    auto res = ctx.format(wchar_value{L'ж'});

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->type() == L"wchar_t");
    BOOST_CHECK(res->val() == L"'ж' (0x00000436)");
}


BOOST_AUTO_TEST_SUITE_END()


}
