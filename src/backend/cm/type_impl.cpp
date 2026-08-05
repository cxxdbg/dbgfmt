
/// \file type_impl.cpp
/// Contains implementation of the type_impl class.

#include "type_impl.hpp"
#include "type_context_impl.hpp"
#include "pointer_type_impl.hpp"
#include <cm/cxx/print.hpp>


namespace dbgfmt::backend::cm {


dbgfmt::backend::type_context_impl * type_impl::ctx() const {
    return &fmtcm_ctx();
}


std::string type_impl::name() const {
    std::ostringstream str;
    ::cm::cxx::print_type(str, get_cm_type());
    return str.str();
}


const type_impl * type_impl::pointer_type() const {
    auto cm_ptr = fmtcm_ctx().cm().get_or_create_ptr_type(get_cm_type());
    return fmtcm_ctx().get_or_create_type(cm_ptr);
}


size_t type_impl::size() const {
    return fmtcm_ctx().dbg().type_size(get_cm_type().type());
}


}
