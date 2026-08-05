
/// \file libcxx_tree_formatter.cpp
/// Contains implementation of formatter for libc++ tree containers.

#include "libcxx_ns_utils.hpp"
#include "libcxx_tree_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "../../template_name.hpp"


namespace dbgfmt {


bool libcxx_tree_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "set") ||
           is_libcxx_record(v, "multiset") ||
           is_libcxx_record(v, "map") ||
           is_libcxx_record(v, "multimap");
}


std::size_t libcxx_tree_formatter::size(const value & val) const {
    return static_cast<std::size_t>(val["__tree_"]["__pair3_"]["__value_"].as_uint64());
}


static value tree_min(const value & x) {
    auto res = x;
    while ((*res)["__left_"].as_uint64() != 0) {
        res = (*res)["__left_"];
    }

    return res;
}


static bool tree_is_left_child(const value & x) {
    return x.as_uint64() == (*((*x)["__parent_"]))["__left_"].as_uint64();
}


static value tree_next(const value & x, const value_type & node_ptr_type) {
    auto res = x;

    if ((*res)["__right_"].as_uint64() != 0) {
        return tree_min((*res)["__right_"]);
    }

    while (!tree_is_left_child(res)) {
        res = (*res)["__parent_"].cast(node_ptr_type);
    }

    return (*res)["__parent_"].cast(node_ptr_type);
}


value libcxx_tree_formatter::element(const value & val, std::size_t index) const {
    auto tree = val["__tree_"];

    // getting type of pointer to real node type
    auto tree_node_t_name = make_libcxx_type_name("__tree_node", tree);
    std::string node_tname = make_template_inst_name(tree_node_t_name, {tree.type().template_param_name(0), "void *"});
    auto node_ptr_type = ctx().find_type(node_tname).pointer();

    // getting pointers to first node
    value first_node = tree["__begin_node_"].cast(node_ptr_type);

    value node = first_node;

    for (size_t i = 0; i < index; ++i) {
        node = tree_next(node, node_ptr_type);
    }

    auto res = (*node.cast(node_ptr_type))["__value_"];

    std::string tname = val.template_name();
    if (is_libcxx_record(val, "map") || is_libcxx_record(val, "multimap")) {
        res = res["__cc"];
    }

    return res;
}


std::wstring libcxx_tree_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}



bool libcxx_tree_iterator_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "__tree_iterator") ||
           is_libcxx_record(v, "__tree_const_iterator");
}


value libcxx_tree_iterator_formatter::get_value_pointer(const value & it) const {
    auto tree_node_t_name = make_libcxx_type_name("__tree_node", it);
    std::string tname = make_template_inst_name(tree_node_t_name, {it.type().template_param_name(0), "void *"});
    auto node_ptr_type = ctx().find_type(tname).pointer();
    value casted_node = *(it["__ptr_"].cast(node_ptr_type));
    value val = casted_node["__value_"];

    // if value has __cc field then this is __value_type wrapper for map and we should return pointer to __cc
    if (val.is_record() && val.has_field("__cc")) {
        val = val["__cc"];
    }

    return val.ptr();
}



bool libcxx_map_iterator_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "__map_iterator");
}


value libcxx_map_iterator_formatter::convert(const value & it) const {
    return it["__i_"];
}


}
