
/// \file std_list_formatter.hpp
/// Contains definition of the std_list_formatter class.

#pragma once

#include "../container_formatter.hpp"
#include "../container_iterator_formatter.hpp"


namespace dbgfmt {


/// Base class for std list formatters. Contains common logic.
class std_list_formatter: public container_formatter {
public:
    /// Constructs list formatter
    std_list_formatter(std::string n_next):
        next_name_{std::move(n_next)} {}

private:
    /// Returns list size
    std::size_t size(const value & val) const override;

    /// Returns value object for list element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns node type name for list value
    virtual std::string node_type_name(const value & val) const = 0;

    /// Returns value containing root list node
    virtual value root_node(const value & val) const = 0;

    /// Returns node value
    virtual value node_value(const value & node, const std::string & tname) const = 0;

    std::string next_name_;                 ///< Name of node next field
};


}
