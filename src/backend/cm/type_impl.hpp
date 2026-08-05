
/// \file type_impl.hpp
/// Contains definition of the type_impl class.

#pragma once

#include <cm/cm.hpp>
#include "dbgfmt/type_impl_common.hpp"


namespace dbgfmt::backend::cm {

class type_context_impl;


/// Base class for all type implementations in the fmtcm library
class type_impl: public dbgfmt::type_impl_common {
public:
    /// Constructs type implementation with specified reference to type context
    type_impl(type_context_impl & tctx): tctx_(tctx) {}

    /// Returns pointer to formatters type context
    dbgfmt::backend::type_context_impl * ctx() const override;

    /// Returns pointer to fmtcm type context
    type_context_impl & fmtcm_ctx() const { return tctx_; }

    /// Returns name of type
    std::string name() const override;

    /// Returns type implementation representing pointer to this type
    const type_impl * pointer_type() const override;

    /// Returns source position of definition of type. Returns invalid
    /// source position if not available
    dbgfmt::backend::source_position def_pos() const override {
        auto decl = get_cm_type().cast<::cm::context_type>();
        if (!decl) {
            return {};
        }

        auto pos = decl->loc();

        if (pos) {
            return {pos.file()->path().string(), pos.line()};
        } else {
            return {};
        }
    }

    /// Returns size of type in bytes
    size_t size() const override;

    /// Returns CM qual type
    virtual ::cm::qual_type get_cm_type() const = 0;

private:
    type_context_impl & tctx_;  ///< Reference to type context
};



/// Base class for all type implementations in the fmtcm library
/// typed with CM type
template <typename CMType>
class cm_type_impl: public type_impl {
public:
    /// Consturcts type implementation with specified reference to type context and CM qual type
    cm_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<CMType> & qt):
        type_impl(tctx), cm_type_{qt} {}

protected:
    /// Returns typed CM qual type
    auto & cm_type() const {
        return cm_type_;
    }

    /// Returns CM qual type
    ::cm::qual_type get_cm_type() const override { return cm_type_; }

private:
    ::cm::qual_type_t<CMType> cm_type_;     ///< CM qual type
};


}


