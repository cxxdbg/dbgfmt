
/// \file enum_type_impl.hpp
/// Contains definition of the enum_type_impl class.

#pragma once

#include "type_impl.hpp"
#include <cm/cm.hpp>
#include "dbgfmt/backend/enum_type_impl.hpp"
#include "dbgfmt/backend/format_error.hpp"


namespace dbgfmt::backend::cm {


/// FMT enum type implementation on top of CM enum type
class enum_type_impl: public cm_type_impl<::cm::enum_type>,
                      virtual public dbgfmt::backend::enum_type_impl {
public:
    /// Constructs enum type implementation with specified reference to type context
    enum_type_impl(type_context_impl & tctx, const ::cm::qual_type_t<::cm::enum_type> & cmt):
        cm_type_impl<::cm::enum_type>(tctx, cmt) {}

    /// Returns name of type
    std::string name() const override {
        return cm_type().name();
    }

    /// Returns number of enum items
    std::size_t items_size() const override {
        return cm_type().type()->items().size();
    }

    /// Returns name of enum item with specified index
    std::string item_name(std::size_t index) const override {
        auto cmt = cm_type().type();
        if (index >= cmt->items().size()) {
            std::ostringstream msg;
            msg << "item index " << index << " for enum " << cmt->name()
                << " is out of range";
            throw dbgfmt::backend::format_error{msg.str()};
        }

        return cmt->items()[index].name;
    }

    /// Returns value of enum item with specified index as signed integer
    std::int64_t item_value(std::size_t index) const override {
        auto cmt = cm_type().type();
        if (index >= cmt->items().size()) {
            std::ostringstream msg;
            msg << "item index " << index << " for enum " << cmt->name()
                << " is out of range";
            throw dbgfmt::backend::format_error{msg.str()};
        }

        return cmt->items()[index].value;
    }
};


}


