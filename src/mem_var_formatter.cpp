
/// \file mem_var_formatter.cpp
/// Contains implementation of the mem_var_formatter class.

#include "mem_var_formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


fmt_result_ref mem_var_formatter::format(const value & val) {
    source_position def_pos;
    auto fld_name = val.mem_var_ptr_field_name_and_def_pos(def_pos);
    std::ostringstream res;

    if (!fld_name.empty()) {
        res << '&' << fld_name;
    } else {
        res << val.as_uint64();
    }

    auto res_str = res.str();
    std::wstring wres{res_str.begin(), res_str.end()};
    return make_fmt_result<fixed_fmt_result>(wres,
                                             val.wqual_type_name(),
                                             def_pos,
                                             val.type().def_pos());
}


}
