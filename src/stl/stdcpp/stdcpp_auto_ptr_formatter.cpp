
/// \file stdcpp_auto_ptr_formatter.cpp
/// Contains implementation of the stdcpp_auto_ptr_formatter class.

#include "stdcpp_auto_ptr_formatter.hpp"


namespace dbgfmt {


bool stdcpp_auto_ptr_formatter::accepts(const value & v) const {
    return v.is_record() && v.template_name() == "std::auto_ptr";
}


value stdcpp_auto_ptr_formatter::get_pointer(const value & v) const {
    return v["_M_ptr"];
}


}
