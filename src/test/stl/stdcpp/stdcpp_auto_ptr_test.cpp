
/// \file stdcpp_auto_ptr_test.cpp
/// Contains unit tests for libstdc++ auto_ptr formatter.

#include "../../builtin_value.hpp"
#include "../../fmt_test_fixture.hpp"
#include "../../pointer_value.hpp"
#include "../../record_value.hpp"
#include "../../util.hpp"
#include "dbgfmt/context.hpp"
#include <boost/test/unit_test.hpp>


namespace dbgfmt::test {


/// Makes auto_ptr record for specitied pointer value
static record_value make_auto_ptr(const pointer_value & ptr) {
    // creating std::auto_ptr type if not created
    auto std = ptr.ctx().cm().get_or_create_namespace("std");
    auto auto_ptr = std->get_or_create_template_record("auto_ptr", cm::record_kind::class_, false, "T");

    auto auto_ptr_inst = auto_ptr->find_instantiation(ptr.type()->base());
    if (!auto_ptr_inst) {
        // creating new instance
        auto_ptr_inst = auto_ptr->create_instantiation(ptr.type()->base());
        auto_ptr_inst->create_field("_M_ptr", ptr.type());
        ptr.ctx().dbg().make_def_rec_layout(auto_ptr_inst);
    }

    record_value rec{auto_ptr_inst};
    rec["_M_ptr"] = ptr;
    return rec;
}


BOOST_FIXTURE_TEST_SUITE(stdcpp_auto_ptr_test, fmt_test_fixture)


/// Tests displaying simple auto_ptr
BOOST_AUTO_TEST_CASE(simple) {

    int_value val{10};
    auto ptr = make_auto_ptr(val.ptr());
    auto res = ctx.format(ptr);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(res->type() == L"std::auto_ptr<int>");

    BOOST_REQUIRE(res->childs_size() == 1);
    auto child = res->child_at(0);
    BOOST_REQUIRE(child);
    BOOST_CHECK(child->name() == L"[ptr]");
    BOOST_CHECK(child->val() == addr_str(val.addr()) + L" 10");
    BOOST_CHECK(child->type() == L"int*");
    BOOST_CHECK(child->childs_size() == 1);
}


BOOST_AUTO_TEST_SUITE_END()


}
