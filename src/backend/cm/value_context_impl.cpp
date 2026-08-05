
/// \file value_context_impl.cpp
/// Contains implementation of the value_context_impl class

#include "value_context_impl.hpp"
#include "array_type_impl.hpp"
#include "builtin_type_impl.hpp"
#include "enum_type_impl.hpp"
#include "function_type_impl.hpp"
#include "mem_ptr_type_impl.hpp"
#include "pointer_type_impl.hpp"
#include "record_type_impl.hpp"
#include "typedef_type_impl.hpp"
#include "dbgfmt/backend/format_error.hpp"
#include <cxxtn/tn.hpp>


namespace dbgfmt::backend::cm {


const type_impl * value_context_impl::dynamic_type_at_addr(uint64_t & addr, const dbgfmt::backend::type_impl * static_type) {
    auto fmtcm_static_type = dynamic_cast<const type_impl*>(static_type);
    assert(fmtcm_static_type && "invalid type passed in dynamic_type_at_addr");

    auto it = dynamic_types_.find(addr);
    if (it == dynamic_types_.end()) {
        return fmtcm_static_type;
    }

    addr = it->second.addr;
    return it->second.type;
}


}
