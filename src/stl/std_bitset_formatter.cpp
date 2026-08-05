
/// \file std_bitset_formatter.cpp
/// Contains implementation of the std_bitset_formatter class.

#include "std_bitset_formatter.hpp"
#include "../container_formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"


namespace dbgfmt {


/// Reads bitset size from bitset value
static size_t read_bitset_size(const value & val) {
    if (val.type().template_param_names_size() < 1) {
        throw dbgfmt::format_error{"can't detect std::bitset size from template parameters"};
    }

    std::istringstream sz_str{val.type().template_param_name(0)};
    uint64_t sz;
    sz_str >> sz;
    if (!sz_str) {
        throw dbgfmt::format_error{"can't parse std::bitset size from template parameter"};
    }

    return sz;
}


/// bitset formatting result
class bitset_fmt_result: public fmt_result {
public:
    /// Constructs bitset formatting result with specified name of data field,
    /// bitset value, bitset size, and values of value and type fields
    bitset_fmt_result(const std::string & d_f_name,
                      const value & bs_val,
                      size_t sz,
                      const std::wstring & val,
                      const std::wstring & type):
        data_field_name_{d_f_name}, bs_val_{bs_val}, bs_size_{sz}, val_{val}, type_{type} {}

    /// Returns value of the value field
    std::wstring val() const override { return val_; }

    /// Returns value of type field
    std::wstring type() const override { return type_; }

    /// Returns source position of definition of object that is contained in address
    /// specied by value. Always returns empty source position
    source_position val_pos() const override { return {}; }

    /// Returns source position of definition of value type
    source_position type_pos() const override { return bs_val_.type().def_pos(); }

    /// Returns number of child nodes
    size_t childs_size() const override { return bs_size_; }

    /// Returns child value at specified index
    named_fmt_result child_at(size_t idx) const override {
        if (idx >= bs_size_) {
            std::ostringstream msg;
            msg << "invalid bitset child index: " << idx;
            throw dbgfmt::format_error{msg.str()};
        }

        uint64_t long_val = 0;
        size_t bit_idx = 0;

        if (bs_val_[data_field_name_].is_array()) {
            // long bitset containing array of long values
            auto arr_elt_size = bs_val_[data_field_name_][0].size() * 8;
            auto arr_idx = idx / arr_elt_size;
            bit_idx = idx % arr_elt_size;
            long_val = bs_val_[data_field_name_][arr_idx].as_uint64();
        } else {
            // short bitset containing single long value
            auto long_size = bs_val_[data_field_name_].size() * 8;
            bit_idx = idx % long_size;
            long_val = bs_val_[data_field_name_].as_uint64();
        }
        
        auto val = ((long_val >> bit_idx) & 1) != 0;
        auto str_val = val ? L"true" : L"false";

        auto res = make_fmt_result<fixed_fmt_result>(str_val,
                                                     L"bool",
                                                     source_position{},
                                                     source_position{});

        std::wostringstream name_str;
        name_str << "[" << idx << "]";

        return named_fmt_result{name_str.str(), {}, res};
    }

private:
    std::string data_field_name_;   ///< Name of data field in bitset record
    value bs_val_;                  ///< Bitset value
    size_t bs_size_;                ///< Bitset size
    std::wstring val_;              ///< Value of name field
    std::wstring type_;             ///< Value of type field
};


fmt_result_ref std_bitset_formatter::format(const value & val) {
    // reading size of bitset
    auto sz = read_bitset_size(val);
    container_formatter::check_size(sz);

    // formatting value
    std::wostringstream val_str;
    val_str << "[size = " << sz << "]";

    auto res = make_fmt_result<bitset_fmt_result>(data_fld_name_,
                                                  val,
                                                  sz,
                                                  val_str.str(),
                                                  disp_type_name(val));
    return container_formatter::make_size_proxy(res, sz);
}


}
