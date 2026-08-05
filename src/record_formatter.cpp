
/// \file record_formatter.cpp
/// Contains implementation of classes for formatting record values.

#include "record_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "functor_formatter.hpp"
#include <sstream>


namespace dbgfmt {


record_fmt_result::record_fmt_result(record_formatter & rec_fmt,
                                     const value & val,
                                     context * ctx):
rec_fmt_{rec_fmt},
val_(val),
ctx_(ctx) {
    std::wostringstream str;

    // writing record address
    if (ctx_->fmt_opts().show_rec_addr() && val_.has_address()) {
        ctx_->write_address(str, val_.address());
        str << L' ';
    }

    // writing fields
    str << "{ ";
    rec_fmt_.start_record_value_format();
    bool first = true;
    for (std::size_t i = 0, e = val.fields_size(); i != e; ++i) {

        if (!first) {
            str << " ";
        } else {
            first = false;
        }

        if (rec_fmt_.skip_next_fields()) {
            // maximum number of displayed fields reached
            str << "...";
            break;
        }

        rec_fmt_.inc_num_record_fields();

        value fld = val_.field_at(i);
        auto fmt_fld = ctx_->format(fld);
        str << val_.wfield_name_at(i) << L"=" << fmt_fld.val();
    }
    rec_fmt_.stop_record_value_format();
    str << " }";

    fval_ = str.str();

    // setting type name
    type_ = val_.wqual_type_name();
}


size_t record_fmt_result::childs_size() const {
    return val_.bases_size() + val_.fields_size();
}


named_fmt_result record_fmt_result::child_at(std::size_t i) const {
    assert(i < childs_size() && "Invalid child index");

    if (i < val_.bases_size()) {
        // direct base
        value base = val_.base_at(i);
        return named_fmt_result{base.wqual_type_name(), {}, ctx_->format(base)};
        // return make_named_fmt_result<record_fmt_result>(base.wqual_type_name(),
        //                                                 {},
        //                                                 rec_fmt_,
        //                                                 base,
        //                                                 ctx_);
    } else {
        // field
        std::size_t fld_index = i - val_.bases_size();
        auto fld = val_.field_at(fld_index);
        return named_fmt_result{val_.wfield_name_at(fld_index),
                                val_.field_def_pos_at(fld_index),
                                ctx_->format(fld)};
    }
}


bool record_formatter::accepts(const value & val) const {
    if (!val.is_record()) {
        return false;
    }

    if (!ctx().fmt_opts().rec_as_functor()) {
        return true;
    }

    // if record has operator() and records as functors flag is set then record will
    // be formatter with functor_formatter
    return !functor_formatter::get_call_operator(val).is_valid();
}


void record_formatter::start_record_value_format() {
    if (rec_value_depth_ == 0) {
        // new record value formatting, reset number of fields
        num_rec_fields_ = 10;
    }

    ++rec_value_depth_;
}


void record_formatter::stop_record_value_format() {
    assert(rec_value_depth_ > 0 && "invalid rec_value_depth_ value");
    --rec_value_depth_;
}


void record_formatter::inc_num_record_fields() {
    if (num_rec_fields_ > 0)
        --num_rec_fields_;
}


bool record_formatter::skip_next_fields() const {
    return rec_value_depth_ > 0 && num_rec_fields_ == 0;
}


}
