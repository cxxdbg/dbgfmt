
/// \file stdcpp_reference_wrapper_formatter.hpp
/// Contains definition of formatter for libstdc++ reference wrapper

#pragma once

#include "dbgfmt/formatter.hpp"
#include "../../proxy_fmt_result.hpp"


namespace dbgfmt {


/// Formatter for libstdc++ reference wrapper
class stdcpp_reference_wrapper_formatter: public formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return v.is_record() && v.template_name() == "std::reference_wrapper";
    }

    /// Formats value. Returns formatting result
    fmt_result_ref format(const value & val) override {
        auto res = ctx().format(val["_M_data"]);
        return make_fmt_result<ot_proxy_fmt_result>(res, val.wqual_type_name());
    }
};


}


