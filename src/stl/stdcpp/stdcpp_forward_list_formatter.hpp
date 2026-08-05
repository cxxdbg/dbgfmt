
/// \file stdcpp_forward_list_formatter.hpp
/// Contains definition of libstdc++ forward_list formatting classes.

#pragma once

#include "../std_forward_list_formatter.hpp"
#include "../std_node_iterator_formatter.hpp"
#include "../../template_name.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ forward_list class
class stdcpp_forward_list_formatter: public std_forward_list_formatter {
public:
    /// Constructs formatter
    stdcpp_forward_list_formatter():
        std_forward_list_formatter{"_M_next"} {}

    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return v.is_record() && v.template_name() == "std::forward_list";
    }

private:
    /// Returns value containing list head
    value list_head(const value & list) const override {
        return list["_M_impl"]["_M_head"];
    }

    /// Returns name of type of node for list value
    std::string node_type_name(const value & list) const override {
        return make_template_inst_name("std::_Fwd_list_node", {list.type().template_param_name(0)});
    }

    /// Returns node value
    value node_value(const value & list, const value & node) const override {
        auto storage = node["_M_storage"];
        return *stdcpp_aligned_memory_ptr(ctx(), storage, list.type().template_param_name(0));
    }
};


/// Formatter for libstdc++ forward list iterator
class stdcpp_forward_list_iterator_formatter: public std_node_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return v.is_record() &&
            (v.template_name() == "std::_Fwd_list_iterator" ||
                v.template_name() == "std::_Fwd_list_const_iterator");
    }

private:
    /// Returns name of type of node for iterator value
    std::string node_type_name(const value & it) const override {
        return make_template_inst_name("std::_Fwd_list_node", {it.type().template_param_name(0)});
    }

    /// Returns pointer to node from iterator value
    value node_ptr(const value & it) const override {
        return it["_M_node"];
    }

    /// Returns pointer to node value
    value node_value_ptr(const value & it, const value & node) const override {
        auto storage = node["_M_storage"];
        return stdcpp_aligned_memory_ptr(ctx(), storage, it.type().template_param_name(0));
    }
};


}


