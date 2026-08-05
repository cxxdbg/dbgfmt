
/// \file value_context_impl.cpp
/// Contains implementation of the value_context_impl class.

#include "value_context_impl.hpp"
#include "dbgfmt/temp_value_impl.hpp"


namespace dbgfmt::test {


std::shared_ptr<backend::value_impl> value_context_impl::make_temp_val(const backend::type_impl * type) {
    return std::make_shared<temp_value_impl>(*this, type, memory(), endianness());
}


}
