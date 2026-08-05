
/// \file stdcpp_cxx11_list_formatter.hpp
/// Contains definition of stdc++ c++11 list formatter classes

#pragma once

#include "../std_list_formatter.hpp"
#include "stdcpp_utils.hpp"
#include "../../template_name.hpp"


namespace dbgfmt {


/// \class stdcpp_list_formatter
/// libstdc++ list formatter (C++11 version)
class stdcpp_cxx11_list_formatter: public std_list_formatter {
public:
    /// Constructs formatter
    stdcpp_cxx11_list_formatter():
        std_list_formatter{"_M_next"} {}

    /// Returs true if formatter accepts specified value
    bool accepts(const value & val) const override {
        return val.is_record() && val.template_name() == "std::__cxx11::list";
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
    value node_value(const value & node, const std::string & tname) const override {
        return *stdcpp_aligned_memory_ptr(ctx(), node["_M_storage"], tname);
    }
};


}


