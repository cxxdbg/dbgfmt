
/// \file pointer_type_impl.cpp
/// Contains implementation of the pointer_type_impl class.

#include "pointer_type_impl.hpp"
#include "function_type_impl.hpp"


namespace dbgfmt::backend::cm {


bool pointer_type_impl::can_dereference() const {
    // pointer to void can't be dereferenced
    if (base() == fmtcm_ctx().bt_void()) {
        return false;
    }

    // pointer to function can't be dereferenced
    if (base()->cast<function_type_impl>()) {
        return false;
    }

    return true;
}


}
