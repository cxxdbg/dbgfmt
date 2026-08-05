
/// \file lambda_formatter.cpp
/// Contains implementation of the lambda_formatter class.

#include "lambda_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fmt_result.hpp"
#include "log.hpp"
#include "proxy_fmt_result.hpp"
#include "dbgfmt/format_error.hpp"
#include <algorithm>
#include <sstream>


namespace dbgfmt {


bool lambda_formatter::accepts(const value & v) const {
    if (!record_formatter::accepts(v)) {
        return false;
    }

    const auto & tn = v.type_name();

    // checking for "<lambda(...>' name (g++)
    if (tn.size() >= 9 &&
        tn.substr(0, 8) == "<lambda(" &&
        tn.back() == '>') {

        return true;
    }


    // Clang does not generate special name for lambda, it uses generic '(anonymous class)' name.
    // New gcc versions generate type without name, in such case LLDB returns '(unnamed struct)' name.
    // Checking for that names and trying find operator() in these types. If opearator() is found
    // then consider that the type is a lambda type.
    if ((v.type_name() == "(anonymous class)" || v.type_name() == "(anonymous struct)" ||
         v.type_name() == "(unnamed class)" || v.type_name() == "(unnamed struct)") &&
        get_call_operator(v).is_valid()) {
        return true;
    }

    return false;
}


}
