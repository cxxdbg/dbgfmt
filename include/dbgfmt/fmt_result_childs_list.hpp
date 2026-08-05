
/// \file fmt_result_childs_list.hpp
/// Contains definition of the fmt_result_childs_list class.

#pragma once

#include "fmt_result.hpp"
#include "format_error.hpp"
#include <vector>


namespace dbgfmt {


/// Represents fixed list of child nodes of formatting result.
/// Each value in list is lazy i. e. calcualted on demand.
class fmt_result_childs_list {
    /// Represents definition of child node of fixed formatting result
    class child {
    public:
        /// Constructs child with specified name
        child(const std::wstring & nm): name_{nm} {}

        /// Destroys object
        virtual ~child() = default;

        /// Returns named formatting result for child
        virtual named_fmt_result value() const = 0;

        /// Returns child name
        auto & name() const { return name_; }

    private:
        std::wstring name_;     ///< Child name
    };

    /// Represents definition of child node with value calculated with functor
    template <typename F>
    class func_child: public child {
    public:
        /// Constructs child with specified name and functor
        func_child(const std::wstring & nm, F f): child{nm}, func_{f} {}

        /// Destroys object
        virtual ~func_child() = default;

        /// Returns value of child
        named_fmt_result value() const override {
            fmt_result_ref res;

            try {
                res = func_();
            }
            catch(format_error & err) {
                res = make_error_fmt_result(err);
            }

            // we always use invalid source position for predefined
            // childs for now. It seems there are not cases when we
            // need to define source position for such values
            return named_fmt_result{name(), {}, res};
        }

    private:
        F func_;                ///< Functor value
    };

public:
    /// Returns number of child nodes
    size_t size() const { return childs_.size(); }

    /// Returns child value at specified index. Calls functor passed
    /// to add method and convert format_error exceptions to error
    /// messages
    named_fmt_result at(size_t idx) const {
        assert(idx < childs_.size() && "Invalid child index");
        return childs_[idx]->value();
    }

    /// Adds child that returns value calculated with specified functor
    template <typename F>
    void add(const std::wstring & nm, const F & f) {
        childs_.push_back(std::make_unique<func_child<F>>(nm, f));
    }

private:
    /// Vector of child definitions
    std::vector<std::unique_ptr<child>> childs_;
};


}


