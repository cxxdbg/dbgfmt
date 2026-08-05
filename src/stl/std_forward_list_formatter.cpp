
/// \file std_forward_list_formatter.cpp
/// Contains implementation of formatters for std::forward_list and related classes.

#include "std_forward_list_formatter.hpp"
#include "stdcpp/stdcpp_utils.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/format_error.hpp"
#include "../template_name.hpp"


namespace dbgfmt {


size_t std_forward_list_formatter::size(const value & val) const {
    value node = list_head(val)[next_name_];
    if (node.as_uint64() == 0) {
        return 0;
    }

    std::size_t sz = 0;
    auto node_ptr_type = ctx().find_type(node_type_name(val)).pointer();

    while (node.as_uint64() != 0) {
        node = (*node.cast(node_ptr_type))[next_name_];
        ++sz;
    }

    return sz;
}


value std_forward_list_formatter::element(const value & val, std::size_t index) const {
    std::size_t cur_index = 0;
    auto node_ptr_type = ctx().find_type(node_type_name(val)).pointer();

    value node = list_head(val)[next_name_];
    while (node.as_uint64() != 0) {
        value casted_node = (*node.cast(node_ptr_type));

        if (cur_index == index) {
            return node_value(val, casted_node);
        }

        node = casted_node[next_name_];
        ++cur_index;
    }

    throw dbgfmt::format_error("can't get std::list item");
}


}
