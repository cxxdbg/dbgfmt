
/// \file pointer_formatter.hpp
/// Contains definition of classes for pointer formatter

#pragma once

#include "dbgfmt/fmt_result.hpp"
#include "dbgfmt/formatter.hpp"
#include "dbgfmt/value.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


/// Result of formatting pointer value
class pointer_fmt_result: public fmt_result {
public:
    /// Formats pointer value
    pointer_fmt_result(const value & v, context * ctx);

    /// Returns value of the value field
    std::wstring val() const override { return fval_; }

    /// Returns value of type field
    std::wstring type() const override { return type_; }

    /// Returns source position of definition of object that is
    /// contained in address specied by value
    source_position val_pos() const override { return val_pos_; }

    /// Returns source position of definition of value type
    source_position type_pos() const override { return {}; }

    /// Returns number of child nodes
    size_t childs_size() const override;

    /// Returns child value. i must be equal to 0.
    named_fmt_result child_at(size_t i) const override;

    /// Returns true if pointer formatter accepts value
    static bool accepts(const value & v) {
        return v.is_pointer();
    }

private:
    value val_;                         ///< Pointer value to format
    context * ctx_;                     ///< Formatting context
    std::wstring fval_;                 ///< Value of the value field
    std::wstring type_;                 ///< Value of the type field
    source_position val_pos_;           ///< Position of definition of pointee
    fmt_result_ref pointee_fval_;       ///< Formatted pointee value
    bool pointee_is_record_ = false;    ///< Is pointee record value?
};


using pointer_formatter = formatter_t<pointer_fmt_result>;


}


