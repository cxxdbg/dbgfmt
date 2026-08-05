
/// \file stdcpp_string.cpp
/// Contains implementation of functions for creating libstdc++ string types and values.

#include "stdcpp_string.hpp"


namespace dbgfmt::test {


cm::record_type * get_stdcxx_string_type(const cm::qual_type & elt_type) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto & dbg = tctx.dbg();

    auto std = cm.get_or_create_namespace("std");
    auto basic_string = std->get_or_create_template_record("basic_string", cm::record_kind::class_, false, "C");

    auto basic_string_inst = basic_string->find_instantiation(elt_type);
    if (basic_string_inst) {
        return basic_string_inst;
    }

    basic_string_inst = basic_string->create_instantiation(elt_type);

    auto rep = basic_string_inst->create_named_record("_Rep");
    rep->create_field("_M_length", cm.bt_unsigned_long());
    rep->create_field("_M_capacity", cm.bt_unsigned_long());
    rep->create_field("_M_refcount", cm.bt_unsigned_long());
    dbg.make_def_rec_layout(rep);

    auto alloc_hider = basic_string_inst->create_named_record("_Alloc_hider");
    alloc_hider->create_field("_M_p", cm.get_or_create_ptr_type(elt_type));
    dbg.make_def_rec_layout(alloc_hider);

    basic_string_inst->create_field("_M_dataplus", alloc_hider);
    dbg.make_def_rec_layout(basic_string_inst);

    return basic_string_inst;
}


/// Gets or creates anonymous recrod type that contains std::string<>::_Rep
/// fields and array of characters
static cm::record_type * get_stdcxx_string_rep_type(const cm::qual_type & char_type, size_t sz) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto & dbg = tctx.dbg();

    auto char_arr_type = cm.get_or_create_arr_type(char_type.type(), sz);
    auto rep = cm.create_record();
    rep->create_field("_M_length", cm.bt_unsigned_long());
    rep->create_field("_M_capacity", cm.bt_unsigned_long());
    rep->create_field("_M_refcount", cm.bt_unsigned_long());
    rep->create_field("_M_data", char_arr_type);
    dbg.make_def_rec_layout(rep);
    return rep;
}


value create_stdcxx_string(const cm::qual_type & char_type, const std::vector<value> & chars) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto rep = make_val(get_stdcxx_string_rep_type(char_type, chars.size()));
    rep["_M_length"] << chars.size();
    rep["_M_capacity"] << (chars.size() + 100);
    rep["_M_refcount"] << 0;

    for (size_t i = 0, e = chars.size(); i < e; ++i) {
        rep["_M_data"][i] << chars[i];
    }

    auto char_ptr_type = cm.get_or_create_ptr_type(char_type);
    auto char_ptr = rep["_M_data"].ptr();
    auto rec = make_val(get_stdcxx_string_type(char_type));
    rec["_M_dataplus"]["_M_p"] << char_ptr;
    return rec;
}


cm::record_type * get_stdcxx_cxx11_basic_string_type(const cm::qual_type & char_type) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();
    auto & dbg = tctx.dbg();

    auto std = cm.get_or_create_namespace("std");
    auto cxx11 = std->get_or_create_namespace("__cxx11");
    auto basic_string = cxx11->get_or_create_template_record("basic_string", cm::record_kind::class_, false, "C");

    auto basic_string_inst = basic_string->find_instantiation(char_type);
    if (basic_string_inst) {
        return basic_string_inst;
    }

    basic_string_inst = basic_string->create_instantiation(char_type);

    auto alloc_hider = basic_string_inst->create_named_record("_Alloc_hider");
    alloc_hider->create_field("_M_p", cm.get_or_create_ptr_type(char_type));
    dbg.make_def_rec_layout(alloc_hider);
    basic_string_inst->create_field("_M_dataplus", alloc_hider);

    basic_string_inst->create_field("_M_string_length", cm.bt_unsigned_long());

    auto buf_union = basic_string_inst->create_record(cm::record_kind::union_);
    auto loc_buf_type = cm.get_or_create_arr_type(char_type.type(), 16);
    buf_union->create_field("_M_local_buf", loc_buf_type);
    buf_union->create_field("_M_allocated_capacity", cm.bt_unsigned_long());
    dbg.make_def_rec_layout(buf_union);
    basic_string_inst->create_field("", buf_union);

    dbg.make_def_rec_layout(basic_string_inst);
    return basic_string_inst;
}


value create_stdcxx_cxx11_string(const cm::qual_type & char_type, const std::vector<value> & chars) {
    auto & tctx = test_context::current_context();
    auto & cm = tctx.cm();

    auto rec = make_val(get_stdcxx_cxx11_basic_string_type(char_type));

    if (chars.size() <= 15) {
        // using local buffer for small string

        for (size_t i = 0, e = chars.size(); i < e; ++i) {
            rec["_M_local_buf"][i] << chars[i];
        }

        for (size_t i = chars.size(), e = 16; i < e; ++i) {

            rec["_M_local_buf"][i] << 0;
        }

        rec["_M_string_length"] << chars.size();
        rec["_M_dataplus"]["_M_p"] << rec["_M_local_buf"].ptr();
    } else {
        // using heap allocated buffer

        auto data = make_arr(char_type, chars.size() + 100);
        for (size_t i = 0, e = chars.size(); i < e; ++i) {
            data[i] << chars[i];
        }

        data[chars.size()] << '\0';

        rec["_M_string_length"] << chars.size();
        rec["_M_dataplus"]["_M_p"] << data.ptr();
        rec["_M_allocated_capacity"] << (chars.size() + 100);
    }

    return rec;
}


}
