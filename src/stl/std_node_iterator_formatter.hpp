
/// \file std_node_iterator_formatter.hpp
/// Contains definition of the std_node_iterator_formatter class.

#pragma once

#include "../container_iterator_formatter.hpp"
#include "dbgfmt/context.hpp"


namespace dbgfmt {


/// Base class for formatters for node based iterators i. e. iterators
/// containing pointer to node
class std_node_iterator_formatter: public container_iterator_formatter {
private:
    /// Returns pointer to value pointed by iterator
    value get_value_pointer(const value & it) const override {
        auto node_ptr_type = ctx().find_type(node_type_name(it)).pointer();
        auto casted_node = *node_ptr(it).cast(node_ptr_type);
        return node_value_ptr(it, casted_node);
    }

    /// Returns name of type of node for iterator value
    virtual std::string node_type_name(const value & it) const = 0;

    /// Returns pointer to node from iterator value
    virtual value node_ptr(const value & it) const = 0;

    /// Returns pointer to node value
    virtual value node_value_ptr(const value & it, const value & node) const = 0;
};


}
