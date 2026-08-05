
#include "stdcpp_unique_ptr_formatter.hpp"
#include "stdcpp_tuple_formatter.hpp"


namespace dbgfmt {


bool stdcpp_unique_ptr_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::unique_ptr";
}


value stdcpp_unique_ptr_formatter::get_pointer(const value & v) const {
    auto child = v["_M_t"];
    if (child.is_record() &&
        (child.template_name() == "std::__uniq_ptr_impl" ||
         child.template_name() == "std::__uniq_ptr_data")) {

        // new implementation
        child = child["_M_t"];
    }
    return stdcpp_tuple_element(child, 0);
}


}
