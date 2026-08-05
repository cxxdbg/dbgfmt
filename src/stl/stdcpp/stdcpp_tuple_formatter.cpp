
/// \file stdcpp_tuple_formatter.cpp
/// Contains implementation of the stdcpp_tuple_formatter class.

#include "stdcpp_tuple_formatter.hpp"
#include "dbgfmt/format_error.hpp"
#include "dbgfmt/context.hpp"
#include <sstream>


namespace dbgfmt {


bool stdcpp_tuple_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::tuple";
}


std::size_t stdcpp_tuple_formatter::size(const value & val) const {
    return stdcpp_tuple_size(val);
}


value stdcpp_tuple_formatter::element(const value & val, std::size_t index) const {
    return stdcpp_tuple_element(val, index);
}


value stdcpp_tuple_element(const value & val, std::size_t index) {
    std::size_t cur_index = 0;
    value impl = val.base_at(0);

    while (cur_index < index) {
        impl = impl.base_at(0);
        ++cur_index;
    }

    std::size_t bindex = (impl.bases_size() > 1 ? 1 : 0);
    auto base = impl.base_at(bindex);
    if (base.has_field("_M_head_impl")) {
        return base["_M_head_impl"];
    } else {
        // Tuple _Head_base does not contain _M_head_impl in case of empty
        // use defined types. It inherits user defined type instead
        if (base.bases_size() == 0) {
            std::ostringstream msg;
            msg << "Can't get base of type '" << base.type_name() << "'";
            throw format_error{msg.str()};
        } else {
            return base.base_at(0);
        }
    }
}


size_t stdcpp_tuple_size(const value & val) {
    if (val.bases_size() == 0)
        return 0;

    std::size_t sz = 1;
    value impl = val.base_at(0);
    while (impl.bases_size() > 1) {
        ++sz;
        impl = impl.base_at(0);
    }

    return sz;
}


}
