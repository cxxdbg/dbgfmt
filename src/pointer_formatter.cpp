
/// \file pointer_formatter.cpp
/// Contains implementation of classes for pointer formatter

#include "pointer_formatter.hpp"
#include "char_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "log.hpp"
#include "utils.hpp"
#include <sstream>


namespace dbgfmt {


pointer_fmt_result::pointer_fmt_result(const value & v, context * ctx):
val_(v),
ctx_(ctx) {
    DBGFMT_LOG_TRACE << "pointer formatter: format '" << v.type_name() << "'";

    auto can_deref = val_.can_dereference();

    std::wostringstream str;

    // extracting pointee name
    std::wstring pname;
    uint64_t poffset = 0;
    std::tie(pname, poffset) = extract_pointee_name(val_);

    // writing pointer address if turned on in format options or
    // if pointer can't be dereferenced and don't have pointee name
    bool write_addr = false;
    if (ctx_->show_ptr_addr() || (!can_deref && pname.empty())) {
        write_addr = true;
        ctx_->write_address(str, val_.as_uint64());
    }

    // writing pointee name
    if (!pname.empty()) {
        // pointer to function or symbol

        if (write_addr) {
            str << ' ';
        }

        write_pointee_name(*ctx_, str, pname, poffset, ctx->hex());
    }

    DBGFMT_LOG_TRACE << "pointer formatter: can_dereference = " << can_deref;

    // write pointee value if possible
    if(can_deref) {
        value pointee = *val_;
        pointee_is_record_ = pointee.is_record();
        pointee_fval_ = ctx_->format(pointee);

        if ((pointee.is_signed() || pointee.is_unsigned()) && pointee.size() == 1) {
            // pointer to character, printing null terminated string

            if (write_addr) {
                str << ' ';
            }

            std::wostringstream fstr;
            write_formatted_zstring(fstr, val_);

            str << fstr.str();

        } else if (pointee.is_wchar()) {
            // pointer to wide character, printing null terminated string

            if (write_addr) {
                str << ' ';
            }

            std::wostringstream fstr;
            write_formatted_zwstring(fstr, val_);

            str << fstr.str();

        } else {
            if (pointee_is_record_ && val_.as_uint64() != pointee.address()) {
                // pointer to dynamic record with address of dynamic value different from pointer value

                // writing dynamic address if turned on in format options
                if (ctx_->show_ptr_addr()) {
                    str << " [dyn addr = ";
                    ctx_->write_address(str, pointee.address());
                    str << "]";
                }
            }

            if (pointee_is_record_) {
                // use pointee type name in type name field to handle dynamic type
                type_ = pointee.ptr().wqual_type_name();
            }

            if (write_addr) {
                str << ' ';
            }

            str << pointee_fval_.val();
        }
    }


    fval_ = str.str();

    // setting type name
    if (type_.empty()) {
        type_ = val_.wqual_type_name();
    }

    // setting position of definition of pointee
    val_pos_ = val_.pointee_def_pos();
}


size_t pointer_fmt_result::childs_size() const {
    // displaying record childs as pointer childs
    if (val_.can_dereference() && pointee_is_record_) {
        assert(pointee_fval_ && "pointee value should be formatted here");
        return pointee_fval_.childs_size();
    }

    return val_.can_dereference() ? 1 : 0;
}


named_fmt_result pointer_fmt_result::child_at(size_t i) const {
    assert(i < childs_size() && "Invalid child index");
    assert(val_.can_dereference() && "pointee value can't be dereferenced");
    assert(pointee_fval_ && "pointee value should be formatted here");

    if (pointee_is_record_) {
        return pointee_fval_.child_at(i);
    }

    return named_fmt_result{{}, {}, pointee_fval_};
}


}
