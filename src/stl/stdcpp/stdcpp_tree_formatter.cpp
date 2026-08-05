
/// \file stdcpp_tree_formatter.cpp
/// Contains implementation of the stdcpp_tree_formatter class.

#include "stdcpp_tree_formatter.hpp"
#include "stdcpp_utils.hpp"
#include "dbgfmt/format_error.hpp"
#include "dbgfmt/context.hpp"
#include "../../template_name.hpp"
#include <iostream>
#include <sstream>


namespace dbgfmt {


bool stdcpp_tree_formatter::accepts(const value & v) const {
    if (!v.is_record())
        return false;

    std::string tname = v.template_name();
    return tname == "std::set" || tname == "std::multiset" ||
           tname == "std::map" || tname == "std::multimap";
}


std::size_t stdcpp_tree_formatter::size(const value & val) const {
    return static_cast<std::size_t>(val["_M_t"]["_M_impl"]["_M_node_count"].as_uint64());
}


/// Returns tree item with specified index
static bool get_tree_item(context & ctx,
                          const std::string & tree_type,
                          const std::string & type,
                          const value & root,
                          std::size_t index,
                          std::size_t & cindex,
                          value & res) {

    std::string tname = make_template_inst_name("std::_Rb_tree_node", {type});
    auto node_ptr_type = ctx.find_type(tname).pointer();
    value casted_root = *root.cast(node_ptr_type);
    value left = casted_root["_M_left"];
    value right = casted_root["_M_right"];

    // checking left
    if (left.as_uint64() != 0) {
        if (get_tree_item(ctx, tree_type, type, left, index, cindex, res))
            return true;
    }

    // checking root
    if (index == cindex) {
        res = casted_root.try_all_field("_M_value_field");
        if (!res.is_valid()) {
            // new std
            value buffer = casted_root["_M_storage"];
            res = *stdcpp_aligned_memory_ptr(ctx, buffer, casted_root.type().template_param_name(0));
        }

        return true;
    }

    ++cindex;

    // checking right
    if (right.as_uint64() != 0) {
        if (get_tree_item(ctx, tree_type, type, right, index, cindex, res))
            return true;
    }

    return false;
}


value stdcpp_tree_formatter::element(const value & val, std::size_t index) const {
    // getting root node
    value tree = val["_M_t"];
    value root = tree["_M_impl"]["_M_header"]["_M_parent"];

    // trying find value
    value res;
    std::size_t cindex = 0;
    if (get_tree_item(ctx(), tree.type_name(), tree.type().template_param_name(1), root, index, cindex, res))
        return res;

    std::ostringstream msg;
    msg << "can't find tree node with index " << index << "\n";
    throw dbgfmt::format_error{msg.str()};
}


bool stdcpp_tree_iterator_formatter::accepts(const value & v) const {
    return v.is_record() &&
           (v.template_name() == "std::_Rb_tree_iterator" ||
            v.template_name() == "std::_Rb_tree_const_iterator");
}


value stdcpp_tree_iterator_formatter::get_value_pointer(const value & it) const {
    std::string tname = make_template_inst_name("std::_Rb_tree_node",
                                                      {it.type().template_param_name(0)});
    auto node_ptr_type = ctx().find_type(tname).pointer();
    value casted_node = *(it["_M_node"].cast(node_ptr_type));
    value res;
    auto fld = casted_node.try_all_field("_M_value_field");

    if (fld.is_valid()) {
        // old std
        res = casted_node.all_field_pointer("_M_value_field");
    } else {
        value buffer = casted_node["_M_storage"];
        res = stdcpp_aligned_memory_ptr(ctx(), buffer, it.type().template_param_name(0));
    }

    return res;
}


}
