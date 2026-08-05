
/// \file proxy_fmt_result.hpp
/// Contains definition of proxy formatting result classes.

#pragma once

#include "dbgfmt/fmt_result.hpp"
#include "dbgfmt/fmt_result_childs_list.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


/// Proxy formatting result that does not change field values of the
/// original formatting result
class proxy_fmt_result: public fmt_result {
public:
    /// Constructs proxy formatting result with specified
    /// reference to original formatting result
    proxy_fmt_result(fmt_result_ref res):
        orig_res_{res} {}


    /// Adds additional child node at the begining of child list
    template <typename F>
    void add_begin_child(const std::wstring & nm, const F & f) {
        begin_childs_.add(nm, f);
    }

    /// Adds additional child node at the end of child list
    template <typename F>
    void add_end_child(const std::wstring & nm, const F & f) {
        end_childs_.add(nm, f);
    }

    /// Returns value of the value field. Redirects call to the original result.
    std::wstring val() const override { return orig_res_.val(); }

    /// Returns value of type field. Redirects call to the original result.
    std::wstring type() const override { return orig_res_.type(); }

    /// Returns source position of definition of object that is
    /// contained in address specied by value
    source_position val_pos() const override { return orig_res_.val_pos(); }

    /// Returns source position of definition of value type
    source_position type_pos() const override { return orig_res_.type_pos(); }

    /// Returns number of child nodes.
    /// Returns number of child nodes in the original result plus
    /// number of additional begin and end nodes
    size_t childs_size() const override {
        return begin_childs_.size() + orig_res_.childs_size() + end_childs_.size();
    }

    /// Returns child value at specified index.
    /// Returns value from list of begin childs + list of childs in the
    /// original result + list of end childs
    named_fmt_result child_at(size_t idx) const override {
        assert(idx < childs_size() && "Invalid child index");

        if (idx < begin_childs_.size()) {
            return begin_childs_.at(idx);
        }

        idx -= begin_childs_.size();

        if (idx < orig_res_.childs_size()) {
            return orig_res_.child_at(idx);
        }

        idx -= orig_res_.childs_size();
        assert(idx < end_childs_.size() && "Invalid child index logic");
        return end_childs_.at(idx);
    }

protected:
    /// Returns reference to the original result
    fmt_result_ref orig_res() const {
        return orig_res_;
    }

private:
    fmt_result_ref orig_res_;       ///< Reference to original formatting result

    /// Additional child nodes at the beginning of child list
    fmt_result_childs_list begin_childs_;

    /// Additional child nodes at the end of child list
    fmt_result_childs_list end_childs_;
};


/// Proxy fmt result that overrides value field
class ov_proxy_fmt_result: public proxy_fmt_result {
public:
    /// Constructs proxy fmt result with specified overriden value field
    ov_proxy_fmt_result(fmt_result_ref res,
                        std::wstring v,
                        const source_position & vpos):
        proxy_fmt_result(res), val_{v}, val_pos_{vpos} {}

    /// Returns overriden value of the value field
    std::wstring val() const override { return val_; }

    /// Returns source position of definition of object that is contained in address
    /// specied by value
    source_position val_pos() const override { return val_pos_; }

private:
    std::wstring val_;          ///< Value of the value field
    source_position val_pos_;   ///< Definition position of value pointed by this node
};


/// Proxy fmt result that overrides value of type field
class ot_proxy_fmt_result: public proxy_fmt_result {
public:
    /// Creates proxy fmt result with specified overriden value of type field
    ot_proxy_fmt_result(fmt_result_ref res, std::wstring t):
        proxy_fmt_result(res), type_{t} {}

    /// Return overriden value of the type field
    std::wstring type() const override { return type_; }

private:
    std::wstring type_;     ///< Value of the type field
};


}


