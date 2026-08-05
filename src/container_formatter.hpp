
/// \file container_formatter.hpp
/// Contains definition of container_formatter class.

#pragma once

#include "dbgfmt/formatter.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


class container_formatter;


/// Result of formatting container value
class container_fmt_result: public fmt_result {
public:
    static constexpr size_t invalid_capacity = SIZE_MAX;

    container_fmt_result(context & ctx,
                         container_formatter & fmt,
                         value v,
                         const std::wstring & fmt_val,
                         size_t sz,
                         const std::wstring & disp_t_name):
        ctx_{ctx}, fmt_{fmt}, val_{v}, fmt_val_{fmt_val},
        size_{sz}, disp_type_name_{disp_t_name} {}

    /// Returns value of the value field
    std::wstring val() const override { return fmt_val_; }

    /// Returns value of type field
    std::wstring type() const override { return disp_type_name_; }

    /// Always returns invalid source position
    source_position val_pos() const override { return {}; }

    /// Returns source position of definition of value type
    source_position type_pos() const override { return val_.type().def_pos(); }

    /// Returns number of child nodes
    size_t childs_size() const override { return size_; }

    /// Returns child value at specified index. Call get_child_at
    /// and converts format_error exception to formatted values
    named_fmt_result child_at(size_t idx) const override;

private:
     context & ctx_;                ///< Reference to formatting context
     container_formatter & fmt_;    ///< Reference to formatter
     value val_;                    ///< Container
     std::wstring fmt_val_;         ///< Value of the value field
     size_t size_;                  ///< Size of container
     std::wstring disp_type_name_;  ///< Type name displayed for user
};


/// Custom formatter for generic container types
class container_formatter: public formatter {
    friend class container_fmt_result;

public:
    static const std::size_t max_container_size = 10000;

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override;

    /// Checks size and throws format error if size is greater
    /// than max_container_size
    static void check_size(std::size_t sz);

    /// Creates proxy for displaying additional size container child
    static fmt_result_ref make_size_proxy(fmt_result_ref res, size_t sz);

protected:
    /// Returns true if container should display [size] child
    virtual bool display_size() const { return true; }

    /// Returns true if container has capacity property. Default
    /// implementation returns false
    virtual bool has_capacity() const;

    /// Returns container capacity. Should be called only if
    /// has_capacity returned true. Default implementation calls assert(false).
    virtual std::size_t capacity(const value &) const;

    /// Transforms result. Container formatter calls this virtual function
    /// before adding size and capacity fields to formatting result.
    /// That allows inherited classes to add custom childs between
    /// size/capacity and container elements. Default implementation
    /// returns result passed to it
    virtual fmt_result_ref transform_result(const value & val, const fmt_result_ref & res) {
        return res;
    }

    /// Returns container size
    virtual std::size_t size(const value & val) const = 0;

    /// Returns value object for container element with specified index
    virtual value element(const value & val, std::size_t index) const = 0;

    /// Returns type name that should be displayed for user. Default is
    /// value qual type name
    virtual std::wstring disp_type_name(const value & val) const;

    /// Formats value. Default implementation formats
    /// value as [size = s, capacity = c]
    virtual std::wstring format_value(const value & val) const;
};


}


