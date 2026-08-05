
/// \file array_formatter.cpp
/// Contains implementation of the array_formatter class.

#include "array_formatter.hpp"


namespace dbgfmt {


std::size_t array_formatter::size(const value & val) const {
    return val.array_size();
}


value array_formatter::element(const value & val, std::size_t index) const {
    return val.array_at(index);
}


}
