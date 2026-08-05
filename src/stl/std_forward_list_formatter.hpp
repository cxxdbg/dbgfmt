
/// \file std_forward_list_formatter.hpp
/// Contains definitions of formatters for std::forward_list and related classes.

#pragma once

#include "std_node_iterator_formatter.hpp"
#include "../container_formatter.hpp"
#include "../container_iterator_formatter.hpp"


namespace dbgfmt {


/// Base class for forward list formatters. Contains common logic.
class std_forward_list_formatter: public container_formatter {
public:
    /// Constructs formatter
    /// \param n_name - name of the next field in list node
    std_forward_list_formatter(std::string n_name):
        next_name_{std::move(n_name)} {}

private:
    /// Returns container size
    size_t size(const value & val) const override;

    /// Returns value object for container element with specified index
    value element(const value & val, std::size_t index) const override;

    /// Returns value containing list head
    virtual value list_head(const value & list) const = 0;

    /// Returns name of type of node for list value
    virtual std::string node_type_name(const value & list) const = 0;

    /// Returns node value
    virtual value node_value(const value & list, const value & node) const = 0;

    std::string next_name_;         ///< Name of the next field in list node
};


}
