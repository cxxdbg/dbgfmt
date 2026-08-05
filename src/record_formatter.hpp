
/// \file record_formatter.hpp
/// Contains definitions of classes for formatting record values.

#pragma once

#include "dbgfmt/fmt_result.hpp"
#include "dbgfmt/formatter.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


class record_formatter;


/// Result of formatting record value
class record_fmt_result: public fmt_result {
public:
    /// Constructor, formats record value
    record_fmt_result(record_formatter & rec_fmt,
                      const value & val,
                      context * ctx);

    /// Returns value of the value field
    std::wstring val() const override { return fval_; }

    /// Returns value of type field
    std::wstring type() const override { return type_; }

    /// Always returns invalid source position
    source_position val_pos() const override { return {}; }

    /// Returns source position of definition of value type
    source_position type_pos() const override { return val_.type().def_pos(); }

    /// Returns number of child nodes
    size_t childs_size() const override;

    /// Returns child value at specified index. Call get_child_at
    /// and converts format_error exception to formatted values
    named_fmt_result child_at(size_t idx) const override;

private:
    record_formatter & rec_fmt_;    ///< Reference to record formatter
    value val_;                     ///< Record value
    context * ctx_;                 ///< Pointer to formatting context
    std::wstring fval_;             ///< Value of the value field
    std::wstring type_;             ///< Value of the type field
};


/// Record formatter
class record_formatter: public formatter {
    friend class record_fmt_result;

public:
    /// Formats value
    fmt_result_ref format(const value & val) override {
        return make_fmt_result<record_fmt_result>(*this, val, &ctx());
    }

    /// Returns true if formatter accepts value
    bool accepts(const value & v) const override;

private:
    /// Starts formatting record value
    void start_record_value_format();

    /// Stops record value formatting
    void stop_record_value_format();

    /// Increases number of formatted record fields
    void inc_num_record_fields();

    /// Returns true if next record fields should be skipped
    bool skip_next_fields() const;

    size_t rec_value_depth_ = 0;        ///< Current record value depth
    size_t num_rec_fields_ = 10;        ///< Number of allowed record fields to display
};


}


