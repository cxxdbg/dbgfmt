
/// \file libcxx_valarray_formatter.cpp
/// Contains implementation of formatters for libc++ valarray and related classes.

#include "libcxx_ns_utils.hpp"
#include "libcxx_valarray_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


bool libcxx_valarray_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "valarray");
}


size_t libcxx_valarray_formatter::size(const value & val) const {
    return static_cast<size_t>(val["__end_"] - val["__begin_"]);
}


value libcxx_valarray_formatter::element(const value & val, std::size_t index) const {
    return *(val["__begin_"] + index);
}


std::wstring libcxx_valarray_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}



bool libcxx_slice_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "slice");
}


fmt_result_ref libcxx_slice_formatter::format(const value & val) {
    auto offs = val["__start_"].as_uint64();
    auto size = val["__size_"].as_uint64();
    auto stride = val["__stride_"].as_uint64();
    std::wostringstream val_str;
    val_str << "[offset = " << offs << ", size = " << size << ", stride = " << stride << "]";

    auto disp_tname = replace_libcxx_type_name(val);

    auto res = std::make_unique<fixed_fmt_result>(val_str.str(),
                                                  disp_tname,
                                                  source_position{},
                                                  source_position{});

    res->add_child(L"[offset]", [offs] {
        std::wostringstream str;
        str << offs;
        return make_fmt_result<fixed_fmt_result>(str.str(),
                                                 L"std::size_t",
                                                 source_position{},
                                                 source_position{});
    });
    res->add_child(L"[size]", [size] {
        std::wostringstream str;
        str << size;
        return make_fmt_result<fixed_fmt_result>(str.str(),
                                                 L"std::size_t",
                                                 source_position{},
                                                 source_position{});
    });
    res->add_child(L"[stride]", [stride] {
        std::wostringstream str;
        str << stride;
        return make_fmt_result<fixed_fmt_result>(str.str(),
                                                 L"std::size_t",
                                                 source_position{},
                                                 source_position{});
    });

    return make_ref(std::move(res));
}


bool libcxx_slice_array_formatter::accepts(const value & v) const {
    return is_libcxx_record(v, "slice_array");
}


size_t libcxx_slice_array_formatter::size(const value & val) const {
    return static_cast<size_t>(val["__size_"].as_uint64());
}


value libcxx_slice_array_formatter::element(const value & val, std::size_t index) const {
    auto stride = val["__stride_"].as_uint64();
    return *(val["__vp_"] + index * stride);
}


std::wstring libcxx_slice_array_formatter::disp_type_name(const value & val) const {
    return replace_libcxx_type_name(val);
}



}
