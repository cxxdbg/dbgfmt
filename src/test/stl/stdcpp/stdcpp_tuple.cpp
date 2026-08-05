
/// \file stdcpp_tuple.cpp
/// Contains implementation of helper functions for creating mock
/// libstdc++ tuples.

#include "stdcpp_tuple.hpp"
#include <algorithm>
#include <ranges>


namespace dbgfmt::test {


/// Calculates number of fields for record type and its base records
static size_t calc_record_fields(const cm::record_type * rtype) {
    auto sz = std::ranges::distance(rtype->fields());
    auto bases = rtype->base_recs();
    return std::accumulate(std::ranges::begin(bases), std::ranges::end(bases), sz, [](auto && csz, auto && base) {
        return csz + calc_record_fields(base);
    });
}


/// Returns true if type is empty record
static bool is_empty_record(const cm::const_qual_type & type) {
    auto rtype = type.cast<cm::record_type>();
    if (!rtype) {
        return false;
    }

    return calc_record_fields(rtype.type()) == 0;
}


/// Makes tuple head base record type for specified qual type
cm::record_type * get_tuple_head_base_type(const cm::qual_type & t) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto std = cm.get_or_create_namespace("std");
    auto Head_base = std->get_or_create_template_record("_Head_base", cm::record_kind::class_, false, "T");

    auto Head_base_inst = Head_base->find_instantiation(t);
    if (Head_base_inst) {
        return Head_base_inst;
    }

    // creating _Head_base instance if not created yet
    Head_base_inst = Head_base->create_instantiation(t);

    auto rtype = t.type()->cast<cm::record_type>();
    if (rtype && calc_record_fields(rtype) == 0) {
        // _Head_base for empty records should be derived from these records
        assert(rtype && "Empty record tuple type is not a record");
        Head_base_inst->add_base(rtype);
    } else {
        Head_base_inst->create_field("_M_head_impl", t);
    }

    tctx.dbg().make_def_rec_layout(Head_base_inst);

    return Head_base_inst;
}


value make_stdcpp_tuple_head_base(const value & v) {
    auto Head_base_inst = get_tuple_head_base_type(val_type(v));
    auto rec = make_val(Head_base_inst);

    if (!is_empty_record(val_type(v))) {
        rec["_M_head_impl"] << v;
    }

    return rec;
}


}
