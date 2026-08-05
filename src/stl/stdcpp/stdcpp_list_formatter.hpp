
/// \file stdcpp_list_formatter.hpp
/// Contains definition of stdc++ list formatter classes

#pragma once

#include "../std_list_formatter.hpp"
#include "../std_node_iterator_formatter.hpp"
#include "stdcpp_utils.hpp"
#include "../../container_iterator_formatter.hpp"
#include "../../template_name.hpp"


namespace dbgfmt {


/// \class stdcpp_list_formatter
/// libstdc++ list formatter
class stdcpp_list_formatter: public std_list_formatter {
public:
    /// Constructs formatter
    stdcpp_list_formatter():
        std_list_formatter{"_M_next"} {}

    /// Returs true if formatter accepts specified value
    bool accepts(const value & val) const override {
        return val.is_record() && val.template_name() == "std::list";
    }

private:
    /// Returns node type name for list value
    std::string node_type_name(const value & val) const override {
        return make_template_inst_name("std::_List_node", {val.type().template_param_name(0)});
    }

    /// Returns value containing root list node
    value root_node(const value & val) const override {
        return val["_M_impl"]["_M_node"];
    }

    /// Returns node value
    value node_value(const value & node, const std::string &) const override {
        return node["_M_data"];
    }
};


/// \class stdcpp_list_iterator_formatter
/// libstdc++ list iterator formatter
class stdcpp_list_iterator_formatter: public std_node_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return v.is_record() &&
               (v.template_name() == "std::_List_iterator" ||
                v.template_name() == "std::_List_const_iterator");
    }

private:
    /// Returns node type name for list value
    std::string node_type_name(const value & val) const override {
        return make_template_inst_name("std::_List_node", {val.type().template_param_name(0)});
    }

    /// Returns pointer to node from iterator value
    value node_ptr(const value & it) const override {
        return it["_M_node"];
    }

    /// Returns pointer to value containing in node
    value node_value_ptr(const value & it, const value & node) const override {
        // std::__cxx11::list iterator
        auto cxx11_storage = node.try_all_field("_M_storage");
        if (cxx11_storage.is_valid()) {
            return stdcpp_aligned_memory_ptr(ctx(), cxx11_storage, it.type().template_param_name(0));
        }

        return node.all_field_pointer("_M_data");
    }
};


}


