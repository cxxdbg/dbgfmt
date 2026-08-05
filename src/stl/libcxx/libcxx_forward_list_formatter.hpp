
/// \file libcxx_forward_list_formatter.hpp
/// Contains definition of libc++ forward_list formatting classes.

#pragma once

#include "libcxx_ns_utils.hpp"
#include "../std_forward_list_formatter.hpp"
#include "../std_node_iterator_formatter.hpp"
#include "dbgfmt/format_error.hpp"
#include <cxxtn/type_name_parser.hpp>
#include <cxxtn/type_name.hpp>
#include "../../template_name.hpp"


namespace dbgfmt {


/// Formatter for libc++ forward_list class
class libcxx_forward_list_formatter: public std_forward_list_formatter {
public:
    /// Constructs formatter
    libcxx_forward_list_formatter():
        std_forward_list_formatter{"__next_"} {}

    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "forward_list");
    }

private:
    /// Returns value containing list head
    value list_head(const value & list) const override {
        return list["__before_begin_"]["__value_"];
    }

    /// Returns name of type of node for list value
    std::string node_type_name(const value & list) const override {
        auto t_name = make_libcxx_type_name("__forward_list_node", list);
        return make_template_inst_name(t_name, {list.type().template_param_name(0), "void*"});
    }

    /// Returns node value
    value node_value(const value & list, const value & node) const override {
        return node["__value_"];
    }

    /// Returns type name that should be displayed for user
    std::wstring disp_type_name(const value & val) const override {
        return replace_libcxx_type_name(val);
    }
};


/// Formatter for libstdc++ forward list iterator
class libcxx_forward_list_iterator_formatter: public std_node_iterator_formatter {
public:
    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "__forward_list_iterator") ||
               is_libcxx_record(v, "__forward_list_const_iterator");
    }

private:
    /// Returns name of type of node for iterator value
    std::string node_type_name(const value & it) const override {
        std::istringstream str{it.type().template_param_name(0)};
        auto node_ptr_tn = cxxtn::parse_type_name(str);
        auto pnode_ptr_tn = dynamic_cast<const cxxtn::pointer_type_name*>(node_ptr_tn.type());
        if (!pnode_ptr_tn) {
            std::ostringstream msg;
            msg << "node ptr is not a pointer type: '" << it["__ptr_"].type_name() << "'";
            throw format_error{msg.str()};
        }

        auto base_tn = pnode_ptr_tn->base().type();
        std::ostringstream res;
        base_tn->write(res, cxxtn::type_name_write_style::clang);
        return res.str();
    }

    /// Returns pointer to node from iterator value
    value node_ptr(const value & it) const override {
        return it["__ptr_"];
    }

    /// Returns pointer to node value
    value node_value_ptr(const value & it, const value & node) const override {
        return node["__value_"].ptr();
    }
};


}
