
/// \file std_tuple_formatter.cpp
/// Contains implementation of the std_tuple_formatter class.

#include "std_tuple_formatter.hpp"
#include "dbgfmt/context.hpp"


namespace dbgfmt {


std::wstring std_tuple_formatter::format_value(const value & val) const {
    std::wostringstream str;
    str << L"<";

    for (size_t i = 0, e = size(val); i < e; ++i) {
        if (i != 0) {
            str << L", ";
        }

        auto fval = ctx().format(element(val, i));
        str << fval.val();
    }

    str << L">";

    return str.str();
}


}

