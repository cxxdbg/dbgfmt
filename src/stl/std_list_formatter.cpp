
/// \file std_list_formatter.cpp
/// Contains implementation of the std_list_formatter class.

#include "std_list_formatter.hpp"
#include "stdcpp/stdcpp_utils.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/format_error.hpp"
#include "../template_name.hpp"


namespace dbgfmt {


std::size_t std_list_formatter::size(const value & val) const {
    std::size_t sz = 0;
    auto n_t_name = node_type_name(val);

    auto r_node = root_node(val);
    value node = r_node[next_name_];
    while (node.as_uint64() != r_node.address()) {
        auto node_ptr_type = ctx().find_type(n_t_name).pointer();
        node = (*node.cast(node_ptr_type))[next_name_];
        ++sz;
    }

    return sz;
}


value std_list_formatter::element(const value & val, std::size_t index) const {
    std::size_t cur_index = 0;
    auto n_t_name = node_type_name(val);

    auto r_node = root_node(val);
    value node = r_node[next_name_];
    while (node.as_uint64() != r_node.address()) {
        auto node_ptr_type = ctx().find_type(n_t_name).pointer();
        value casted_node = (*node.cast(node_ptr_type));

        if (cur_index == index) {
            return node_value(casted_node, val.type().template_param_name(0));
        }

        node = casted_node[next_name_];
        ++cur_index;
    }

    throw dbgfmt::format_error("can't get std::list item");
}


}
