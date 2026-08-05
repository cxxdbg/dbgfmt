
/// \file utils.cpp
/// Contains implementation of formatting utitlity functions

#include "utils.hpp"
#include "dbgfmt/context.hpp"
#include "print.hpp"


namespace dbgfmt {


std::tuple<std::wstring, uint64_t> extract_pointee_name(const value & val) {
    assert(val.is_pointer() && "value should be a pointer");

    auto func_name = val.wfunction_name();
    std::wstring obj_name;
    uint64_t obj_offset = 0;

    // lldb sometimes extracts only "operator()" for lambdas.
    // In such case we should try use symbol name
    if (!func_name.empty() && func_name != L"operator()") {
        obj_name = func_name;
        obj_offset = val.function_offset();
    } else {
        // if function name is empty then try
        // use & + symbol name
        obj_name = val.wsymbol_name();
        if (!obj_name.empty()) {
            obj_offset = val.symbol_offset();
        } else {
            obj_name = func_name;
            obj_offset = val.function_offset();
        }
    }

    return std::make_tuple(obj_name, obj_offset);
}


void write_pointee_name(context & ctx,
                        std::wostream & str,
                        const std::wstring & name,
                        uint64_t offset,
                        bool is_hex) {

    str << "&" << name;

    if (offset == 0) {
        return;
    }

    str << " + ";

    if (is_hex) {
        print_hex(str, offset, ctx.address_size(), ctx.min_hex_size());
    } else {
        str << offset;
    }
}


std::wstring replace(const std::wstring & str, const std::wstring & sub_str, const std::wstring & replace_str) {
    std::wstring res = str;
    while (true) {
        auto pos = res.find(sub_str);
        if (pos == std::wstring::npos) {
            break;
        }

        res.replace(pos, sub_str.size(), replace_str);
    }

    return res;
}


}
