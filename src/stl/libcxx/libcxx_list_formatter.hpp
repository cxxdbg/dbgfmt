
/// \file libcxx_formatter.hpp
/// Contains definition of formatters for libc++ list and related classes.

#pragma once

#include "libcxx_ns_utils.hpp"
#include "../std_list_formatter.hpp"
#include "../std_node_iterator_formatter.hpp"
#include "../../template_name.hpp"


namespace dbgfmt {


/// libc++ list formatter
class libcxx_list_formatter: public std_list_formatter {
public:
    /// Constructs formatter
    libcxx_list_formatter():
        std_list_formatter{"__next_"} {}

    /// Returs true if formatter accepts specified value
    bool accepts(const value & val) const override {
        return is_libcxx_record(val, "list");
    }

private:
    /// Returns node type name for list value
    std::string node_type_name(const value & val) const override {
        auto t_name = make_libcxx_type_name("__list_node", val);
        return make_template_inst_name(t_name, {val.type().template_param_name(0), "void*"});
    }

    /// Returns value containing root list node
    value root_node(const value & val) const override {
        return val["__end_"];
    }

    /// Returns node value
    value node_value(const value & node, const std::string &) const override {
        return node["__value_"];
    }

    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override {
        return replace_libcxx_type_name(val);
    }
};


/// libc++ list iterator formatter
class libcxx_list_iterator_formatter: public std_node_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "__list_iterator") ||
               is_libcxx_record(v, "__list_const_iterator");
    }

private:
    /// Returns node type name for list value
    std::string node_type_name(const value & val) const override {
        auto t_name = make_libcxx_type_name("__list_node", val);
        return make_template_inst_name(t_name, {val.type().template_param_name(0), "void*"});
    }

    /// Returns pointer to node from iterator value
    value node_ptr(const value & it) const override {
        return it["__ptr_"];
    }

    /// Returns pointer to value containing in node
    value node_value_ptr(const value & it, const value & node) const override {
        return node["__value_"].ptr();
    }
};


}
