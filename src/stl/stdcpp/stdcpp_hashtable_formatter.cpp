
/// \file stdcpp_hashtable_formatter.cpp
/// Contains implementation of the stdcpp_hashtable_formatter class.

#include "stdcpp_hashtable_formatter.hpp"
#include "stdcpp_utils.hpp"
#include "dbgfmt/context.hpp"
#include "../../template_name.hpp"


namespace dbgfmt {


bool stdcpp_hashtable_formatter::accepts(const value & v) const {
    if (!v.is_record())
        return false;

    const std::string & tname = v.template_name();
    return tname == "std::unordered_set" ||
           tname == "std::unordered_multiset" ||
           tname == "std::unordered_map" ||
           tname == "std::unordered_multimap";
}


std::size_t stdcpp_hashtable_formatter::size(const value & val) const {
    return static_cast<std::size_t>(val["_M_h"]["_M_element_count"].as_uint64());
}


value stdcpp_hashtable_formatter::element(const value & val, std::size_t index) const {
    value htable = val["_M_h"];
    value node = htable["_M_before_begin"]["_M_nxt"];

    const std::string & tname = htable.type().template_param_name(1);
    const std::string & node_tname = make_template_inst_name("std::_Hash_node_value_base", {tname});
    auto node_ptr_type = ctx().find_type(node_tname).pointer();

    for (std::size_t i = 0; i < index; ++i) {
        node = (*node.cast(node_ptr_type))["_M_nxt"];
    }

    value buffer = (*node.cast(node_ptr_type))["_M_storage"];
    return *stdcpp_aligned_memory_ptr(ctx(), buffer, tname);
}


bool stdcpp_hashtable_iterator_formatter::accepts(const value & v) const {
    return v.is_record() &&
           (v.template_name() == "std::__detail::_Node_iterator" ||
            v.template_name() == "std::__detail::_Node_const_iterator");
}


value stdcpp_hashtable_iterator_formatter::get_value_pointer(const value & it) const {
    const std::string & tname = it.type().template_param_name(0);
    const std::string & node_tname =
            make_template_inst_name("std::_Hash_node_value_base", {tname});
    auto node_ptr_type = ctx().find_type(node_tname).pointer();
    value buffer = (*it["_M_cur"].cast(node_ptr_type))["_M_storage"];
    return stdcpp_aligned_memory_ptr(ctx(), buffer, tname);
}


}
