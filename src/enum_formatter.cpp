
/// \file enum_formatter.cpp
/// Contains implementation of classes for formatting enum values.

#include "enum_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "print.hpp"
#include <sstream>


namespace dbgfmt {


fmt_result_ref enum_formatter::format(const value & val) {

    // reading value as int64
    std::int64_t v = val.as_int64();

    // looking for enum item name
    std::wstring item_name;
    for (std::size_t i = 0, e = val.enum_items_size(); i < e; ++i) {
        if (val.enum_item_value(i) == v) {
            const auto & iname = val.enum_item_name(i);
            item_name.assign(iname.begin(), iname.end());
            break;
        }
    }

    // writing value to output stream

    std::wostringstream str;

    if (item_name.empty()) {
        // item not found
        if (ctx().hex()) {
            print_hex(str, v, val.size(), ctx().min_hex_size());
        } else {
            str << v;
        }
    } else {
        str << item_name << " (";
        if (ctx().hex()) {
            print_hex(str, v, val.size(), ctx().min_hex_size());
        } else {
            str << v;
        }
        str << ")";
    }

    return make_fmt_result<fixed_fmt_result>(str.str(),
                                             val.wqual_type_name(),
                                             source_position{},
                                             val.type().def_pos());
}


}
