
/// \file stdcpp_queue_formatter.cpp
/// Contains implementation of the stdc++ queue formatter.

#include "stdcpp_queue_formatter.hpp"
#include "dbgfmt/context.hpp"


namespace dbgfmt {


static bool is_queue(const value & v) {
    return v.is_record() && v.template_name() == "std::queue";
}


bool stdcpp_queue_formatter::accepts(const value & v) const {
    return is_queue(v);
}


std::size_t stdcpp_queue_formatter::size(const value & val) const {
    value real_val = is_queue(val) ? val["c"] : val;
    return stdcpp_deque_formatter::size(real_val);
}


value stdcpp_queue_formatter::element(const value & val, std::size_t index) const {
    value real_val = is_queue(val) ? val["c"] : val;
    return stdcpp_deque_formatter::element(real_val, index);
}


}
