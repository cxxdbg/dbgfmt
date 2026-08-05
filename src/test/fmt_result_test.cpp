
/// \file fmt_result_test.cpp
/// Contains unit tests for formatting result classes.

#include "util.hpp"
#include "dbgfmt/fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include "mock.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>


namespace dbgfmt::test {


class mock_fmt_result_bridge: public mock::object<mock_fmt_result_bridge>,
                              public fmt_result_bridge,
                              public ref_counted_impl {
public:
    MOCK_DEFINE_METHOD_0_C(br_get_val, ref_counted_string*())
    MOCK_DEFINE_METHOD_0_C(br_get_type, ref_counted_string*())
    MOCK_DEFINE_METHOD_0_C(br_get_childs_size, size_t())
    MOCK_DEFINE_METHOD_5_C(br_get_child_at, void(size_t,
                                                 ref_counted_string **,
                                                 ref_counted_string **,
                                                 unsigned int *,
                                                 fmt_result_bridge **))
};


class mock_fmt_result: public mock::object<mock_fmt_result>,
                       public fmt_result {
public:
    MOCK_DEFINE_METHOD_0_C(val, std::wstring())
    MOCK_DEFINE_METHOD_0_C(type, std::wstring())
    MOCK_DEFINE_METHOD_0_C(val_pos, source_position())
    MOCK_DEFINE_METHOD_0_C(type_pos, source_position())
    MOCK_DEFINE_METHOD_0_C(childs_size, size_t())
    MOCK_DEFINE_METHOD_1_C(child_at, named_fmt_result(size_t))
};


BOOST_AUTO_TEST_SUITE(fmt_result_test)


/// Tests error handling in fmt_result implementation while getting child
BOOST_AUTO_TEST_CASE(fmt_result_child_error) {
    mock_fmt_result res;

    MOCK_ADD_CALL(res, child_at, [](size_t) -> named_fmt_result {
        throw format_error{"my error"};
    });

    ref_counted_string * name = nullptr;
    ref_counted_string * pos_file = nullptr;
    unsigned int pos_line = 0;
    fmt_result_bridge * cres = nullptr;
    res.br_get_child_at(0, &name, &pos_file, &pos_line, &cres);

    auto name_ref = acquire_string(name);
    auto pos_file_ref = acquire_string(pos_file);
    auto res_ref = acquire_fmt_result(cres);

    BOOST_CHECK(name_ref.read() == L"");
    BOOST_CHECK(!pos_file_ref);
    BOOST_CHECK_EQUAL(pos_line, 0);
    BOOST_CHECK(is_error_res(res_ref, L"<error>"));
}


BOOST_AUTO_TEST_SUITE_END()


}
