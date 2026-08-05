
/// \file fixed_fmt_result.hpp
/// Contains definition of the fixed_fmt_result class.

#pragma once

#include "fmt_result.hpp"
#include "fmt_result_childs_list.hpp"
#include "source_position.hpp"
#include <vector>


namespace dbgfmt {


/// Formatting result that contains predefined values and childs
class fixed_fmt_result: public fmt_result {
public:
    /// Constructs formatting result with specified predefined
    /// values of the value and the type field.
    fixed_fmt_result(std::wstring v,
                     std::wstring t = {},
                     const source_position & vpos = {},
                     const source_position & tpos = {}):
        val_{v}, type_{t}, val_pos_{vpos}, type_pos_{tpos} {}

    /// No copy constructor
    fixed_fmt_result(const fixed_fmt_result &) = delete;

    /// Destroys object
    virtual ~fixed_fmt_result() = default;

    /// Returns value of the value field
    std::wstring val() const override { return val_; }

    /// Returns value of type field
    std::wstring type() const override { return type_; }

    /// Returns source position of definition of object that is
    /// contained in address specied by value
    source_position val_pos() const override { return val_pos_; }

    /// Returns source position of definition of value type
    source_position type_pos() const override { return type_pos_; }

    /// Returns number of child nodes
    size_t childs_size() const override { return childs_.size(); }

    /// Returns child value at specified index. Call get_child_at
    /// and converts format_error exception to formatted values
    named_fmt_result child_at(size_t idx) const override {
        return childs_.at(idx);
    }

    /// Adds child that returns value calculated with specified functor
    template <typename F>
    void add_child(const std::wstring & nm, const F & f) {
        childs_.add(nm, f);
    }

private:
    std::wstring val_;          ///< Value of the value field
    std::wstring type_;         ///< Value of the type field
    source_position val_pos_;   ///< Position of value contained at address
    source_position type_pos_;  ///< Position of type definition in source code

    /// Vector of child definitions
    fmt_result_childs_list childs_;
};


}


