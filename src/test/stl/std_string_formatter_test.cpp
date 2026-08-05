
/// \file std_string_formatter_test.cpp
/// Contains tests for STL string formatters

#include "../test2.hpp"
#include "libcxx/libcxx_string.hpp"
#include "stdcpp/stdcpp_string.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


BOOST_FIXTURE_TEST_SUITE(stdcpp_string_formatter, fmt_test_fixture)


void check_empty_string(const fmt_result_ref & res,
                        const std::wstring & templ_name,
                        const std::wstring & cap) {

    auto type_name = templ_name + L"<char>";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"\"\"");
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE(res->childs_size() == 2);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"0");
    BOOST_CHECK(size->type() == L"std::size_t");

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == cap);
    BOOST_CHECK(capacity->type() == L"std::size_t");
}


BOOST_AUTO_TEST_CASE(empty_stdcxx_string) {
    check_empty_string(ctx.format(create_stdcxx_string(cm.bt_char(), {})), L"std::basic_string", L"100");
}

BOOST_AUTO_TEST_CASE(empty_stdcxx_cxx11_string) {
    check_empty_string(ctx.format(create_stdcxx_cxx11_string(cm.bt_char(), {})), L"std::__cxx11::basic_string", L"15");
}

BOOST_AUTO_TEST_CASE(empty_libcxx_string) {
    check_empty_string(ctx.format(create_libcxx_string(cm.bt_char(), {})), L"std::basic_string", L"22");
}

BOOST_AUTO_TEST_CASE(empty_libcxx_string_al) {
    check_empty_string(ctx.format(create_libcxx_string(cm.bt_char(), {}, true)), L"std::basic_string", L"22");
}


void check_simple_string(const fmt_result_ref & res,
                         const std::wstring & templ_name,
                         const std::wstring & cap) {

    auto type_name = templ_name + L"<char>";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"\"ab\\x01c\"");
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE(res->childs_size() == 6);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"4");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == cap);
    BOOST_CHECK(capacity->type() == L"std::size_t");
    BOOST_CHECK(capacity->childs_size() == 0);

    auto char1 = res->child_at(2);
    BOOST_REQUIRE(char1);
    BOOST_CHECK(char1->name() == L"[0]");
    BOOST_CHECK(char1->val() == L"'a' (97)");
    BOOST_CHECK(char1->type() == L"char");
    BOOST_CHECK(char1->childs_size() == 0);

    auto char2 = res->child_at(3);
    BOOST_REQUIRE(char2);
    BOOST_CHECK(char2->name() == L"[1]");
    BOOST_CHECK(char2->val() == L"'b' (98)");
    BOOST_CHECK(char2->type() == L"char");
    BOOST_CHECK(char2->childs_size() == 0);

    auto char3 = res->child_at(4);
    BOOST_REQUIRE(char3);
    BOOST_CHECK(char3->name() == L"[2]");
    BOOST_CHECK(char3->val() == L"'\\x01' (1)");
    BOOST_CHECK(char3->type() == L"char");
    BOOST_CHECK(char3->childs_size() == 0);

    auto char4 = res->child_at(5);
    BOOST_REQUIRE(char4);
    BOOST_CHECK(char4->name() == L"[3]");
    BOOST_CHECK(char4->val() == L"'c' (99)");
    BOOST_CHECK(char4->type() == L"char");
    BOOST_CHECK(char4->childs_size() == 0);
}


BOOST_AUTO_TEST_CASE(simple_stdcxx_string) {
    std::vector<value> chars;
    chars.push_back(make_char('a'));
    chars.push_back(make_char('b'));
    chars.push_back(make_char(1));
    chars.push_back(make_char('c'));

    auto res = ctx.format(create_stdcxx_string(cm.bt_char(), chars));
    check_simple_string(res, L"std::basic_string", L"104");
}

BOOST_AUTO_TEST_CASE(simple_stdcxx_cxx11_string) {
    std::vector<value> chars;
    chars.push_back(make_char('a'));
    chars.push_back(make_char('b'));
    chars.push_back(make_char(1));
    chars.push_back(make_char('c'));

    auto res = ctx.format(create_stdcxx_cxx11_string(cm.bt_char(), chars));
    check_simple_string(res, L"std::__cxx11::basic_string", L"15");
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string) {
    std::vector<value> chars;
    chars.push_back(make_char('a'));
    chars.push_back(make_char('b'));
    chars.push_back(make_char(1));
    chars.push_back(make_char('c'));

    auto res = ctx.format(create_libcxx_string(cm.bt_char(), chars));
    check_simple_string(res, L"std::basic_string", L"22");
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string_al) {
    std::vector<value> chars;
    chars.push_back(make_char('a'));
    chars.push_back(make_char('b'));
    chars.push_back(make_char(1));
    chars.push_back(make_char('c'));

    auto res = ctx.format(create_libcxx_string(cm.bt_char(), chars, true));
    check_simple_string(res, L"std::basic_string", L"22");
}


void check_long_string(const fmt_result_ref & res, const std::wstring & templ_name) {
    auto type_name = templ_name + L"<char>";

    std::wostringstream val_str;
    val_str << "\"";
    for (int i = 0; i < 100; ++i) {
        val_str << "z";
    }
    val_str << "...\"";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == val_str.str());
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE_EQUAL(res->childs_size(), 1002);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"1000");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == L"1100");
    BOOST_CHECK(capacity->type() == L"std::size_t");
    BOOST_CHECK(capacity->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(long_stdcxx_string) {

    std::vector<value> chars;
    for (int i = 0; i < 1000; ++i) {
        chars.push_back(make_char('z'));
    }
    auto res = ctx.format(create_stdcxx_string(cm.bt_char(), chars));

    check_long_string(res, L"std::basic_string");
}

BOOST_AUTO_TEST_CASE(long_stdcxx_cxx11_string) {

    std::vector<value> chars;
    for (int i = 0; i < 1000; ++i) {
        chars.push_back(make_char('z'));
    }
    auto res = ctx.format(create_stdcxx_cxx11_string(cm.bt_char(), chars));

    check_long_string(res, L"std::__cxx11::basic_string");
}

BOOST_AUTO_TEST_CASE(long_libcxx_string) {

    std::vector<value> chars;
    for (int i = 0; i < 1000; ++i) {
        chars.push_back(make_char('z'));
    }
    auto res = ctx.format(create_libcxx_string(cm.bt_char(), chars));

    check_long_string(res, L"std::basic_string");
}

BOOST_AUTO_TEST_CASE(long_libcxx_string_al) {

    std::vector<value> chars;
    for (int i = 0; i < 1000; ++i) {
        chars.push_back(make_char('z'));
    }
    auto res = ctx.format(create_libcxx_string(cm.bt_char(), chars, true));

    check_long_string(res, L"std::basic_string");
}



/// Tests displaying raw data child
void check_simple_string_raw_data(const fmt_result_ref & res,
                                  const std::wstring & templ_name,
                                  const std::wstring & cap,
                                  const std::wstring & raw_templ_name) {
    auto type_name = templ_name + L"<char>";
    auto raw_type_name = raw_templ_name + L"<char>";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"\"a\"");
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE(res->childs_size() == 4);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"1");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == cap);
    BOOST_CHECK(capacity->type() == L"std::size_t");
    BOOST_CHECK(capacity->childs_size() == 0);

    auto char1 = res->child_at(2);
    BOOST_REQUIRE(char1);
    BOOST_CHECK(char1->name() == L"[0]");
    BOOST_CHECK(char1->val() == L"'a' (97)");
    BOOST_CHECK(char1->type() == L"char");
    BOOST_CHECK(char1->childs_size() == 0);

    auto rd = res->child_at(3);
    BOOST_REQUIRE(rd);
    BOOST_CHECK(rd->name() == L"[Raw Data]");
    BOOST_CHECK(rd->type() == raw_type_name);
}

BOOST_AUTO_TEST_CASE(simple_stdcxx_string_raw_data) {
    auto string = create_stdcxx_string(cm.bt_char(), {make_char('a')});

    ctx.set_show_raw_data(true);
    auto res = ctx.format(string);
    check_simple_string_raw_data(res, L"std::basic_string", L"101", L"std::basic_string");
}

BOOST_AUTO_TEST_CASE(simple_stdcxx_cxx11_string_raw_data) {
    auto string = create_stdcxx_cxx11_string(cm.bt_char(), {make_char('a')});

    ctx.set_show_raw_data(true);
    auto res = ctx.format(string);
    check_simple_string_raw_data(res, L"std::__cxx11::basic_string", L"15", L"std::__cxx11::basic_string");
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string_raw_data) {
    auto string = create_libcxx_string(cm.bt_char(), {make_char('a')});

    ctx.set_show_raw_data(true);
    auto res = ctx.format(string);
    check_simple_string_raw_data(res, L"std::basic_string", L"22", L"std::__1::basic_string");
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string_raw_data_al) {
    auto string = create_libcxx_string(cm.bt_char(), {make_char('a')}, true);

    ctx.set_show_raw_data(true);
    auto res = ctx.format(string);
    check_simple_string_raw_data(res, L"std::basic_string", L"22", L"std::__1::basic_string");
}



/// Tests displaying pointer to std::string
void check_simple_string_ptr(const fmt_result_ref & res,
                             const std::wstring & templ_name,
                             const std::wstring & cap,
                             uint64_t addr) {

    auto type_name = templ_name + L"<char>*";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(addr) + L" \"a\"");
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE(res->childs_size() == 3);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"1");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == cap);
    BOOST_CHECK(capacity->type() == L"std::size_t");
    BOOST_CHECK(capacity->childs_size() == 0);

    auto char1 = res->child_at(2);
    BOOST_REQUIRE(char1);
    BOOST_CHECK(char1->name() == L"[0]");
    BOOST_CHECK(char1->val() == L"'a' (97)");
    BOOST_CHECK(char1->type() == L"char");
    BOOST_CHECK(char1->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(simple_stdcxx_string_ptr) {
    auto string = create_stdcxx_string(cm.bt_char(), {make_char('a')});
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr(res, L"std::basic_string", L"101", string.address());
}

BOOST_AUTO_TEST_CASE(simple_stdcxx_cxx11_string_ptr) {
    auto string = create_stdcxx_cxx11_string(cm.bt_char(), {make_char('a')});
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr(res, L"std::__cxx11::basic_string", L"15", string.address());
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string_ptr) {
    auto string = create_libcxx_string(cm.bt_char(), {make_char('a')});
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr(res, L"std::__1::basic_string", L"22", string.address());
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string_ptr_al) {
    auto string = create_libcxx_string(cm.bt_char(), {make_char('a')}, true);
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr(res, L"std::__1::basic_string", L"22", string.address());
}



/// Tests displaying pointer to string with "Raw Data"
void check_simple_string_ptr_raw_data(const fmt_result_ref & res,
                                      const std::wstring & templ_name,
                                      const std::wstring & cap,
                                      uint64_t addr,
                                      const std::wstring & raw_templ_name) {

    auto type_name = templ_name + L"<char>";
    auto raw_type_name = raw_templ_name + L"<char>";
    auto ptr_type_name = type_name + L"*";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(addr) + L" \"a\"");
    BOOST_CHECK(res->type() == ptr_type_name);
    BOOST_REQUIRE(res->childs_size() == 4);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"1");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == cap);
    BOOST_CHECK(capacity->type() == L"std::size_t");
    BOOST_CHECK(capacity->childs_size() == 0);

    auto char1 = res->child_at(2);
    BOOST_REQUIRE(char1);
    BOOST_CHECK(char1->name() == L"[0]");
    BOOST_CHECK(char1->val() == L"'a' (97)");
    BOOST_CHECK(char1->type() == L"char");
    BOOST_CHECK(char1->childs_size() == 0);

    auto rd = res->child_at(3);
    BOOST_REQUIRE(rd);
    BOOST_CHECK(rd->name() == L"[Raw Data]");
    BOOST_CHECK(rd->type() == raw_type_name);
}

BOOST_AUTO_TEST_CASE(simple_stdcxx_string_ptr_raw_data) {
    auto string = create_stdcxx_string(cm.bt_char(), {make_char('a')});
    ctx.set_show_raw_data(true);
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr_raw_data(res, L"std::basic_string", L"101", string.address(), L"std::basic_string");
}

BOOST_AUTO_TEST_CASE(simple_stdcxx_cxx11_string_ptr_raw_data) {
    auto string = create_stdcxx_cxx11_string(cm.bt_char(), {make_char('a')});
    ctx.set_show_raw_data(true);
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr_raw_data(res, L"std::__cxx11::basic_string", L"15", string.address(), L"std::__cxx11::basic_string");
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string_ptr_raw_data) {
    auto string = create_libcxx_string(cm.bt_char(), {make_char('a')});
    ctx.set_show_raw_data(true);
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr_raw_data(res, L"std::__1::basic_string", L"22", string.address(), L"std::__1::basic_string");
}

BOOST_AUTO_TEST_CASE(simple_libcxx_string_ptr_raw_data_al) {
    auto string = create_libcxx_string(cm.bt_char(), {make_char('a')}, true);
    ctx.set_show_raw_data(true);
    auto res = ctx.format(string.ptr());
    check_simple_string_ptr_raw_data(res, L"std::__1::basic_string", L"22", string.address(), L"std::__1::basic_string");
}


/// Tests formatting std::string value using current locale
void check_unicode_string(const fmt_result_ref & res,
                          const std::wstring & templ_name,
                          const std::wstring & cap,
                          size_t chars_len) {

    auto type_name = templ_name + L"<char>";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"\"Фыв\\nж\"");
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE_EQUAL(res->childs_size(), chars_len + 2);

    std::wostringstream chars_len_str;
    chars_len_str << chars_len;

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == chars_len_str.str());
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == cap);
    BOOST_CHECK(capacity->type() == L"std::size_t");
    BOOST_CHECK(capacity->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(unicode_stdcxx_string) {
    const char *cstr = "Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = strlen(cstr);
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_char(cstr[i]));
    }

    auto string = create_stdcxx_string(cm.bt_char(), chars);
    auto res = ctx.format(string);

    std::wostringstream cap_str;
    cap_str << (chars_len + 100);

    check_unicode_string(res, L"std::basic_string", cap_str.str(), chars_len);
}

BOOST_AUTO_TEST_CASE(unicode_stdcxx_cxx11_string) {
    const char *cstr = "Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = strlen(cstr);
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_char(cstr[i]));
    }

    auto string = create_stdcxx_cxx11_string(cm.bt_char(), chars);
    auto res = ctx.format(string);

    check_unicode_string(res, L"std::__cxx11::basic_string", L"15", chars_len);
}

BOOST_AUTO_TEST_CASE(unicode_libcxx_string) {
    const char *cstr = "Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = strlen(cstr);
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_char(cstr[i]));
    }

    auto string = create_libcxx_string(cm.bt_char(), chars);
    auto res = ctx.format(string);

    check_unicode_string(res, L"std::basic_string", L"22", chars_len);
}

BOOST_AUTO_TEST_CASE(unicode_libcxx_string_al) {
    const char *cstr = "Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = strlen(cstr);
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_char(cstr[i]));
    }

    auto string = create_libcxx_string(cm.bt_char(), chars, true);
    auto res = ctx.format(string);

    check_unicode_string(res, L"std::basic_string", L"22", chars_len);
}



/// Tests formatting std::wstring value using current locale
void check_unicode_wstring(const fmt_result_ref & res,
                           const std::wstring & templ_name,
                           const std::wstring & cap) {

    auto type_name = templ_name + L"<wchar_t>";

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"\"Фыв\\nж\"");
    BOOST_CHECK(res->type() == type_name);
    BOOST_REQUIRE_EQUAL(res->childs_size(), 5 + 2);

    auto size = res->child_at(0);
    BOOST_REQUIRE(size);
    BOOST_CHECK(size->name() == L"[size]");
    BOOST_CHECK(size->val() == L"5");
    BOOST_CHECK(size->type() == L"std::size_t");
    BOOST_CHECK(size->childs_size() == 0);

    auto capacity = res->child_at(1);
    BOOST_REQUIRE(capacity);
    BOOST_CHECK(capacity->name() == L"[capacity]");
    BOOST_CHECK(capacity->val() == cap);
    BOOST_CHECK(capacity->type() == L"std::size_t");
    BOOST_CHECK(capacity->childs_size() == 0);
}

BOOST_AUTO_TEST_CASE(unicode_stdcxx_wstring) {

    const wchar_t *cstr = L"Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = 5;
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_wchar(cstr[i]));
    }

    auto res = ctx.format(create_stdcxx_string(cm.bt_wchar_t(), chars));
    check_unicode_wstring(res, L"std::basic_string", L"105");
}

BOOST_AUTO_TEST_CASE(unicode_stdcxx_cxx11_wstring) {

    const wchar_t *cstr = L"Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = 5;
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_wchar(cstr[i]));
    }

    auto res = ctx.format(create_stdcxx_cxx11_string(cm.bt_wchar_t(), chars));
    check_unicode_wstring(res, L"std::__cxx11::basic_string", L"15");
}

BOOST_AUTO_TEST_CASE(unicode_libcxx_wstring) {

    const wchar_t *cstr = L"Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = 5;
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_wchar(cstr[i]));
    }

    auto res = ctx.format(create_libcxx_string(cm.bt_wchar_t(), chars));
    check_unicode_wstring(res, L"std::basic_string", L"106");
}

BOOST_AUTO_TEST_CASE(unicode_libcxx_wstring_al) {

    const wchar_t *cstr = L"Фыв\nж";

    std::vector<value> chars;
    size_t chars_len = 5;
    for (size_t i = 0, e = chars_len; i < e; ++i) {
        chars.push_back(make_wchar(cstr[i]));
    }

    auto res = ctx.format(create_libcxx_string(cm.bt_wchar_t(), chars, true));
    check_unicode_wstring(res, L"std::basic_string", L"106");
}


BOOST_AUTO_TEST_SUITE_END()


}
