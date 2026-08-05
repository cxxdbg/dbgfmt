
/// \file std_chrono_test.cpp
/// Contains unit tests for formatters for types from <chrono> header

#include "../test2.hpp"
#include "../cm_builder.hpp"
#include <cm/record_type.hpp>
#include "../test_context.hpp"
#include <boost/test/unit_test.hpp>
#include <filesystem>


namespace dbgfmt::test {


namespace {
    struct chrono_cm_info {
        using templ_map = std::map<std::string, cm::template_record_instantiation_type*>;

        cm::named_record_type * system_clock = nullptr;
        cm::named_record_type * steady_clock = nullptr;
        templ_map ratio_insts;
        templ_map duration_insts;
        templ_map system_time_point_insts;
        templ_map steady_time_point_insts;
    };
}


/// Creates code model for chrono types
static chrono_cm_info create_chrono_cm(bool is_libcxx) {
    auto & cm = test_context::current_context().cm();
    auto & dbg = test_context::current_context().dbg();

    chrono_cm_info result;

    struct ratio_info {
        std::string id;
        long long num;
        long long denom;
    };

    std::vector<ratio_info> ratios = {
        {"nanoseconds", 1, 1000000000},
        {"microseconds", 1, 1000000},
        {"milliseconds", 1, 1000},
        {"seconds", 1, 1},
        {"minutes", 60, 1},
        {"hours", 3600, 1},
        {"days", 86400, 1},
        {"weeks", 604800, 1},
        {"months", 2629746, 1},
        {"years", 31556952, 1}
    };

    // getting or creating std namespace
    auto * std_ns = cm.get_or_create_namespace("std");
    if (is_libcxx) {
        std_ns = std_ns->get_or_create_namespace("__1");
    }

    // getting or creating chorno namespace
    auto chrono_ns = std_ns->get_or_create_namespace("chrono");

    // creating system_clock and steady_clock records
    result.system_clock = chrono_ns->create_named_record("system_clock");
    result.steady_clock = chrono_ns->create_named_record("steady_clock");

    // creating ratio, duration and time_point templates
    auto ratio_templ = std_ns->create_template_record("ratio", cm::record_kind::struct_, "Num", "Denom");
    auto duration_templ = chrono_ns->create_template_record("duration", cm::record_kind::struct_, "Rep", "Period");
    auto time_point_templ = chrono_ns->create_template_record("time_point", cm::record_kind::struct_, "Clock", "Duration");

    // creating template instantiaions for each ratio
    for (auto && r : ratios) {
        // creating ratio template instantiation
        auto ratio_inst = ratio_templ->create_instantiation(cm::value{r.num}, cm::value{r.denom});
        result.ratio_insts.emplace(r.id, ratio_inst);
        dbg.make_def_rec_layout(ratio_inst);

        // creating duration template instantiation
        auto duration_inst = duration_templ->create_instantiation(cm.bt_long_long(), ratio_inst);
        const auto & dur_rep_var_name = is_libcxx ? "__rep_" : "__r";
        duration_inst->create_field(dur_rep_var_name, cm.bt_long_long());
        result.duration_insts.emplace(r.id, duration_inst);
        dbg.make_def_rec_layout(duration_inst);

        const auto & tp_rep_var_name = is_libcxx ? "__d_" : "__d";

        // creating time_point template instantiation for system clock
        auto sys_time_point_inst = time_point_templ->create_instantiation(result.system_clock, duration_inst);
        sys_time_point_inst->create_field(tp_rep_var_name, duration_inst);
        result.system_time_point_insts.emplace(r.id, sys_time_point_inst);
        dbg.make_def_rec_layout(sys_time_point_inst);

        // creating time_point template instantiation for steady clock
        auto steady_time_point_inst = time_point_templ->create_instantiation(result.steady_clock, duration_inst);
        steady_time_point_inst->create_field(tp_rep_var_name, duration_inst);
        result.steady_time_point_insts.emplace(r.id, steady_time_point_inst);
        dbg.make_def_rec_layout(steady_time_point_inst);
    }

    return result;
}


/// Creates duration value
static value create_duration(const chrono_cm_info & cm_info, const std::string & id, bool is_libcxx, long long val) {
    auto res = make_val(cm_info.duration_insts.at(id));
    const auto & rep_var_name = is_libcxx ? "__rep_" : "__r";
    res[rep_var_name] << val;
    return res;
}


/// Creates system time_point value from duration value
static value create_system_time_point(const chrono_cm_info & cm_info, const std::string & id, bool is_libcxx, const value & dur) {
    auto res = make_val(cm_info.system_time_point_insts.at(id));
    const auto & rep_var_name = is_libcxx ? "__d_" : "__d";
    res[rep_var_name] << dur;
    return res;
}


/// Creates system time_point value from duration value
static value create_steady_time_point(const chrono_cm_info & cm_info, const std::string & id, bool is_libcxx, const value & dur) {
    auto res = make_val(cm_info.steady_time_point_insts.at(id));
    const auto & rep_var_name = is_libcxx ? "__d_" : "__d";
    res[rep_var_name] << dur;
    return res;
}


BOOST_FIXTURE_TEST_SUITE(std_bitset_formatter_test, fmt_test_fixture)


/// Tests formatting long duration value
static void test_format_long(context & ctx, bool is_libcxx) {
    // system time_point = 1626777020015501197
    // values = 51 y 6 M 19 d 10 h 30 m 20 s 15 ms 501 us 197 ns
    // total values = 51 y 618 M 18828 d 451882 h 27112950 m 1626777020 s 1626777020015 ms 1626777020015501 us 1626777020015501197 ns

    auto cm_info = create_chrono_cm(is_libcxx);
    auto val = create_duration(cm_info, "nanoseconds", is_libcxx, 1626777020015501197);

    auto res = ctx.format(val);
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"1626777020015501197ns (51y 6M 19d 10h 30m 20s 15ms 501us 197ns)");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 12);

    auto child0 = res->child_at(0);
    BOOST_CHECK(child0->name() == L"[rep]");
    BOOST_CHECK(child0->val() == L"1626777020015501197");

    auto child1 = res->child_at(1);
    BOOST_CHECK(child1->name() == L"[ratio]");
    BOOST_CHECK(child1->val() == L"1 / 1000000000 (nanoseconds)");

    auto child2 = res->child_at(2);
    BOOST_CHECK(child2->name() == L"[normalized]");
    BOOST_CHECK(child2->val() == L"51y 6M 19d 10h 30m 20s 15ms 501us 197ns");

    auto child3 = res->child_at(3);
    BOOST_CHECK(child3->name() == L"[years]");
    BOOST_CHECK(child3->val() == L"51");

    auto child4 = res->child_at(4);
    BOOST_CHECK(child4->name() == L"[months]");
    BOOST_CHECK(child4->val() == L"618");

    auto child5 = res->child_at(5);
    BOOST_CHECK(child5->name() == L"[days]");
    BOOST_CHECK(child5->val() == L"18828");

    auto child6 = res->child_at(6);
    BOOST_CHECK(child6->name() == L"[hours]");
    BOOST_CHECK(child6->val() == L"451882");

    auto child7 = res->child_at(7);
    BOOST_CHECK(child7->name() == L"[minutes]");
    BOOST_CHECK(child7->val() == L"27112950");

    auto child8 = res->child_at(8);
    BOOST_CHECK(child8->name() == L"[seconds]");
    BOOST_CHECK(child8->val() == L"1626777020");

    auto child9 = res->child_at(9);
    BOOST_CHECK(child9->name() == L"[milliseconds]");
    BOOST_CHECK(child9->val() == L"1626777020015");

    auto child10 = res->child_at(10);
    BOOST_CHECK(child10->name() == L"[microseconds]");
    BOOST_CHECK(child10->val() == L"1626777020015501");

    auto child11 = res->child_at(11);
    BOOST_CHECK(child11->name() == L"[nanoseconds]");
    BOOST_CHECK(child11->val() == L"1626777020015501197");
}

BOOST_AUTO_TEST_CASE(test_format_long_libstdcpp) {
    test_format_long(ctx, false);
}

BOOST_AUTO_TEST_CASE(test_format_long_libcxx) {
    test_format_long(ctx, true);
}


/// Tests formatting negative long duration value
static void test_format_neg_long(context & ctx, bool is_libcxx) {
    auto cm_info = create_chrono_cm(is_libcxx);
    auto val = create_duration(cm_info, "nanoseconds", is_libcxx, -1626777020015501197);

    auto res = ctx.format(val);
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"-1626777020015501197ns (-51y -6M -19d -10h -30m -20s -15ms -501us -197ns)");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 12);

    auto child0 = res->child_at(0);
    BOOST_CHECK(child0->name() == L"[rep]");
    BOOST_CHECK(child0->val() == L"-1626777020015501197");

    auto child1 = res->child_at(1);
    BOOST_CHECK(child1->name() == L"[ratio]");
    BOOST_CHECK(child1->val() == L"1 / 1000000000 (nanoseconds)");

    auto child2 = res->child_at(2);
    BOOST_CHECK(child2->name() == L"[normalized]");
    BOOST_CHECK(child2->val() == L"-51y -6M -19d -10h -30m -20s -15ms -501us -197ns");

    auto child3 = res->child_at(3);
    BOOST_CHECK(child3->name() == L"[years]");
    BOOST_CHECK(child3->val() == L"-51");

    auto child4 = res->child_at(4);
    BOOST_CHECK(child4->name() == L"[months]");
    BOOST_CHECK(child4->val() == L"-618");

    auto child5 = res->child_at(5);
    BOOST_CHECK(child5->name() == L"[days]");
    BOOST_CHECK(child5->val() == L"-18828");

    auto child6 = res->child_at(6);
    BOOST_CHECK(child6->name() == L"[hours]");
    BOOST_CHECK(child6->val() == L"-451882");

    auto child7 = res->child_at(7);
    BOOST_CHECK(child7->name() == L"[minutes]");
    BOOST_CHECK(child7->val() == L"-27112950");

    auto child8 = res->child_at(8);
    BOOST_CHECK(child8->name() == L"[seconds]");
    BOOST_CHECK(child8->val() == L"-1626777020");

    auto child9 = res->child_at(9);
    BOOST_CHECK(child9->name() == L"[milliseconds]");
    BOOST_CHECK(child9->val() == L"-1626777020015");

    auto child10 = res->child_at(10);
    BOOST_CHECK(child10->name() == L"[microseconds]");
    BOOST_CHECK(child10->val() == L"-1626777020015501");

    auto child11 = res->child_at(11);
    BOOST_CHECK(child11->name() == L"[nanoseconds]");
    BOOST_CHECK(child11->val() == L"-1626777020015501197");
}

BOOST_AUTO_TEST_CASE(test_format_neg_long_libstdcpp) {
    test_format_neg_long(ctx, false);
}

BOOST_AUTO_TEST_CASE(test_format_neg_long_libcxx) {
    test_format_neg_long(ctx, true);
}


/// Tests formatting seconds duration with small number
static void test_format_seconds_small(context & ctx, bool is_libcxx) {
    auto cm_info = create_chrono_cm(is_libcxx);
    auto val = create_duration(cm_info, "seconds", is_libcxx, 50);

    auto res = ctx.format(val);

    BOOST_REQUIRE(res);
    BOOST_CHECK(res->val() == L"50s");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 12);

    auto child0 = res->child_at(0);
    BOOST_CHECK(child0->name() == L"[rep]");
    BOOST_CHECK(child0->val() == L"50");

    auto child1 = res->child_at(1);
    BOOST_CHECK(child1->name() == L"[ratio]");
    BOOST_CHECK(child1->val() == L"1 (seconds)");

    auto child2 = res->child_at(2);
    BOOST_CHECK(child2->name() == L"[normalized]");
    BOOST_CHECK(child2->val() == L"50s");

    auto child3 = res->child_at(3);
    BOOST_CHECK(child3->name() == L"[years]");
    BOOST_CHECK(child3->val() == L"0");

    auto child4 = res->child_at(4);
    BOOST_CHECK(child4->name() == L"[months]");
    BOOST_CHECK(child4->val() == L"0");

    auto child5 = res->child_at(5);
    BOOST_CHECK(child5->name() == L"[days]");
    BOOST_CHECK(child5->val() == L"0");

    auto child6 = res->child_at(6);
    BOOST_CHECK(child6->name() == L"[hours]");
    BOOST_CHECK(child6->val() == L"0");

    auto child7 = res->child_at(7);
    BOOST_CHECK(child7->name() == L"[minutes]");
    BOOST_CHECK(child7->val() == L"0");

    auto child8 = res->child_at(8);
    BOOST_CHECK(child8->name() == L"[seconds]");
    BOOST_CHECK(child8->val() == L"50");

    auto child9 = res->child_at(9);
    BOOST_CHECK(child9->name() == L"[milliseconds]");
    BOOST_CHECK(child9->val() == L"50000");

    auto child10 = res->child_at(10);
    BOOST_CHECK(child10->name() == L"[microseconds]");
    BOOST_CHECK(child10->val() == L"50000000");

    auto child11 = res->child_at(11);
    BOOST_CHECK(child11->name() == L"[nanoseconds]");
    BOOST_CHECK(child11->val() == L"50000000000");

}

BOOST_AUTO_TEST_CASE(test_format_seconds_small_libstdcpp) {
    test_format_seconds_small(ctx, false);
}

BOOST_AUTO_TEST_CASE(test_format_seconds_small_libcxx) {
    test_format_seconds_small(ctx, true);
}


/// Tests formatting steady time point
static void test_format_steady_time_point(context & ctx, bool is_libcxx) {
    auto cm_info = create_chrono_cm(is_libcxx);
    auto dur = create_duration(cm_info, "nanoseconds", is_libcxx, 1626777020015501197);
    auto val = create_steady_time_point(cm_info, "nanoseconds", is_libcxx, dur);

    auto tp_res = ctx.format(val);
    BOOST_REQUIRE(tp_res);
    BOOST_CHECK(tp_res->val() == L"epoch+1626777020015501197ns (51y 6M 19d 10h 30m 20s 15ms 501us 197ns)");
    BOOST_REQUIRE_EQUAL(tp_res->childs_size(), 1);

    auto res = tp_res->child_at(0);
    BOOST_REQUIRE(res);
    BOOST_CHECK(res->name() == L"[duration]");
    BOOST_CHECK(res->val() == L"1626777020015501197ns (51y 6M 19d 10h 30m 20s 15ms 501us 197ns)");
    BOOST_REQUIRE_EQUAL(res->childs_size(), 12);

    auto child0 = res->child_at(0);
    BOOST_CHECK(child0->name() == L"[rep]");
    BOOST_CHECK(child0->val() == L"1626777020015501197");

    auto child1 = res->child_at(1);
    BOOST_CHECK(child1->name() == L"[ratio]");
    BOOST_CHECK(child1->val() == L"1 / 1000000000 (nanoseconds)");

    auto child2 = res->child_at(2);
    BOOST_CHECK(child2->name() == L"[normalized]");
    BOOST_CHECK(child2->val() == L"51y 6M 19d 10h 30m 20s 15ms 501us 197ns");

    auto child3 = res->child_at(3);
    BOOST_CHECK(child3->name() == L"[years]");
    BOOST_CHECK(child3->val() == L"51");

    auto child4 = res->child_at(4);
    BOOST_CHECK(child4->name() == L"[months]");
    BOOST_CHECK(child4->val() == L"618");

    auto child5 = res->child_at(5);
    BOOST_CHECK(child5->name() == L"[days]");
    BOOST_CHECK(child5->val() == L"18828");

    auto child6 = res->child_at(6);
    BOOST_CHECK(child6->name() == L"[hours]");
    BOOST_CHECK(child6->val() == L"451882");

    auto child7 = res->child_at(7);
    BOOST_CHECK(child7->name() == L"[minutes]");
    BOOST_CHECK(child7->val() == L"27112950");

    auto child8 = res->child_at(8);
    BOOST_CHECK(child8->name() == L"[seconds]");
    BOOST_CHECK(child8->val() == L"1626777020");

    auto child9 = res->child_at(9);
    BOOST_CHECK(child9->name() == L"[milliseconds]");
    BOOST_CHECK(child9->val() == L"1626777020015");

    auto child10 = res->child_at(10);
    BOOST_CHECK(child10->name() == L"[microseconds]");
    BOOST_CHECK(child10->val() == L"1626777020015501");

    auto child11 = res->child_at(11);
    BOOST_CHECK(child11->name() == L"[nanoseconds]");
    BOOST_CHECK(child11->val() == L"1626777020015501197");
}

BOOST_AUTO_TEST_CASE(test_format_steady_time_point_libstdcpp) {
    test_format_steady_time_point(ctx, false);
}

BOOST_AUTO_TEST_CASE(test_format_steady_time_point_libcxx) {
    test_format_steady_time_point(ctx, true);
}


/// Tests formatting system time point
static void test_format_system_time_point(context & ctx, bool is_libxx) {
    auto cm_info = create_chrono_cm(is_libxx);
    auto dur = create_duration(cm_info, "nanoseconds", is_libxx, 1626777020015501197);
    auto val = create_system_time_point(cm_info, "nanoseconds", is_libxx, dur);

    auto res = ctx.format(val);
    BOOST_REQUIRE(res);

    // we can't check local time representation because it depends on current timezone
    // BOOST_CHECK(res->val() == L"2021-07-20 17:30:20.015501197");

    BOOST_REQUIRE_EQUAL(res->childs_size(), 11);

    auto local_res = res->child_at(0);
    BOOST_REQUIRE(local_res);
    BOOST_CHECK(local_res->name() == L"[UTC]");
    BOOST_CHECK(local_res->val() == L"2021-07-20 10:30:20.015501197");

    auto dur_res = res->child_at(1);
    BOOST_REQUIRE(dur_res);
    BOOST_CHECK(dur_res->name() == L"[duration]");
    BOOST_CHECK(dur_res->val() == L"1626777020015501197ns (51y 6M 19d 10h 30m 20s 15ms 501us 197ns)");
    BOOST_REQUIRE_EQUAL(dur_res->childs_size(), 12);

    {
        auto child0 = dur_res->child_at(0);
        BOOST_CHECK(child0->name() == L"[rep]");
        BOOST_CHECK(child0->val() == L"1626777020015501197");

        auto child1 = dur_res->child_at(1);
        BOOST_CHECK(child1->name() == L"[ratio]");
        BOOST_CHECK(child1->val() == L"1 / 1000000000 (nanoseconds)");

        auto child2 = dur_res->child_at(2);
        BOOST_CHECK(child2->name() == L"[normalized]");
        BOOST_CHECK(child2->val() == L"51y 6M 19d 10h 30m 20s 15ms 501us 197ns");

        auto child3 = dur_res->child_at(3);
        BOOST_CHECK(child3->name() == L"[years]");
        BOOST_CHECK(child3->val() == L"51");

        auto child4 = dur_res->child_at(4);
        BOOST_CHECK(child4->name() == L"[months]");
        BOOST_CHECK(child4->val() == L"618");

        auto child5 = dur_res->child_at(5);
        BOOST_CHECK(child5->name() == L"[days]");
        BOOST_CHECK(child5->val() == L"18828");

        auto child6 = dur_res->child_at(6);
        BOOST_CHECK(child6->name() == L"[hours]");
        BOOST_CHECK(child6->val() == L"451882");

        auto child7 = dur_res->child_at(7);
        BOOST_CHECK(child7->name() == L"[minutes]");
        BOOST_CHECK(child7->val() == L"27112950");

        auto child8 = dur_res->child_at(8);
        BOOST_CHECK(child8->name() == L"[seconds]");
        BOOST_CHECK(child8->val() == L"1626777020");

        auto child9 = dur_res->child_at(9);
        BOOST_CHECK(child9->name() == L"[milliseconds]");
        BOOST_CHECK(child9->val() == L"1626777020015");

        auto child10 = dur_res->child_at(10);
        BOOST_CHECK(child10->name() == L"[microseconds]");
        BOOST_CHECK(child10->val() == L"1626777020015501");

        auto child11 = dur_res->child_at(11);
        BOOST_CHECK(child11->name() == L"[nanoseconds]");
        BOOST_CHECK(child11->val() == L"1626777020015501197");
    }

    auto child3 = res->child_at(2);
    BOOST_CHECK(child3->name() == L"[year]");
    BOOST_CHECK(child3->val() == L"2021");

    auto child4 = res->child_at(3);
    BOOST_CHECK(child4->name() == L"[month]");
    BOOST_CHECK(child4->val() == L"7");

    auto child5 = res->child_at(4);
    BOOST_CHECK(child5->name() == L"[day]");
    BOOST_CHECK(child5->val() == L"20");

    auto child6 = res->child_at(5);
    BOOST_CHECK(child6->name() == L"[hour]");

    // we can't check local hour representation because it depends on current timezone
    // BOOST_CHECK(child6->val() == L"17");

    auto child7 = res->child_at(6);
    BOOST_CHECK(child7->name() == L"[minute]");
    BOOST_CHECK(child7->val() == L"30");

    auto child8 = res->child_at(7);
    BOOST_CHECK(child8->name() == L"[second]");
    BOOST_CHECK(child8->val() == L"20");

    auto child9 = res->child_at(8);
    BOOST_CHECK(child9->name() == L"[millisecond]");
    BOOST_CHECK(child9->val() == L"15");

    auto child10 = res->child_at(9);
    BOOST_CHECK(child10->name() == L"[microsecond]");
    BOOST_CHECK(child10->val() == L"501");

    auto child11 = res->child_at(10);
    BOOST_CHECK(child11->name() == L"[nanosecond]");
    BOOST_CHECK(child11->val() == L"197");
}

BOOST_AUTO_TEST_CASE(test_format_system_time_point_libstdcpp) {
    test_format_system_time_point(ctx, false);
}

BOOST_AUTO_TEST_CASE(test_format_system_time_point_libcxx) {
    test_format_system_time_point(ctx, true);
}


BOOST_AUTO_TEST_SUITE_END()


}
