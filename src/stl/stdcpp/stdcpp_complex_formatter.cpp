
/// \file stdcpp_complex_fomatter.cpp
/// Contains implementation of the stdcpp_complex_formatter class.

#include "stdcpp_complex_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/fmt_result.hpp"
#include "../../format_complex.hpp"
#include "dbgfmt/source_position.hpp"
#include <cassert>


namespace dbgfmt {


bool stdcpp_complex_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::complex";
}



fmt_result_ref stdcpp_complex_formatter::format(const value & val) {
    if (val.has_field("_M_value")) {
        return ctx().format_raw(val["_M_value"]);
    } else {
        std::wostringstream str;
        auto real = val["_M_real"];
        auto img = val["_M_imag"];
        write_values_complex(real, img, ctx().hex(), ctx().min_hex_size(), str);
        return make_fmt_result<fixed_fmt_result>(str.str(),
                                                 val.wqual_type_name(),
                                                 source_position{},
                                                 val.type().def_pos());
    }
}


}
