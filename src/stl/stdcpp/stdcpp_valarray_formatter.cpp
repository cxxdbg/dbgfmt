
/// \file stdcpp_valarray_formatter.cpp
/// Contains implementation of the stdcpp_valarray_formatter class.

#include "stdcpp_valarray_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"


namespace dbgfmt {


bool stdcpp_valarray_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::valarray";
}


size_t stdcpp_valarray_formatter::size(const value & val) const {
    return static_cast<size_t>(val["_M_size"].as_uint64());
}


value stdcpp_valarray_formatter::element(const value & val, std::size_t index) const {
    return *(val["_M_data"] + index);
}


bool stdcpp_slice_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::slice";
}


fmt_result_ref stdcpp_slice_formatter::format(const value & val) {
    auto offs = val["_M_off"].as_uint64();
    auto size = val["_M_sz"].as_uint64();
    auto stride = val["_M_st"].as_uint64();
    std::wostringstream val_str;
    val_str << "[offset = " << offs << ", size = " << size << ", stride = " << stride << "]";

    auto res = std::make_unique<fixed_fmt_result>(val_str.str(),
                                                  val.wtype_name(),
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


bool stdcpp_slice_array_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::slice_array";
}


size_t stdcpp_slice_array_formatter::size(const value & val) const {
    return static_cast<size_t>(val["_M_sz"].as_uint64());
}


value stdcpp_slice_array_formatter::element(const value & val, std::size_t index) const {
    auto stride = val["_M_stride"].as_uint64();
    return *(val["_M_array"]["_M_data"] + index * stride);
}


}
