
/// \file type_impl_common.cpp
/// Contains implementation of the type_impl_common class.

#include "dbgfmt/type_impl_common.hpp"
#include "dbgfmt/backend/builtin_type_impl.hpp"
#include "dbgfmt/backend/array_type_impl.hpp"
#include "dbgfmt/backend/enum_type_impl.hpp"
#include "dbgfmt/backend/function_type_impl.hpp"
#include "dbgfmt/backend/mem_ptr_type_impl.hpp"
#include "dbgfmt/backend/pointer_type_impl.hpp"
#include "dbgfmt/backend/record_type_impl.hpp"
#include "dbgfmt/backend/typedef_type_impl.hpp"
#include "dbgfmt/backend/vector_type_impl.hpp"


namespace dbgfmt {



const backend::builtin_type_impl * type_impl_common::try_as_builtin() const {
    return dynamic_cast<const backend::builtin_type_impl*>(this);
}


const backend::array_type_impl * type_impl_common::try_as_array() const {
    return dynamic_cast<const backend::array_type_impl*>(this);
}


const backend::vector_type_impl * type_impl_common::try_as_vector() const {
    return dynamic_cast<const backend::vector_type_impl*>(this);
}


const backend::enum_type_impl * type_impl_common::try_as_enum() const {
    return dynamic_cast<const backend::enum_type_impl*>(this);
}


const backend::function_type_impl * type_impl_common::try_as_function() const {
    return dynamic_cast<const backend::function_type_impl*>(this);
}


const backend::pointer_type_impl * type_impl_common::try_as_pointer() const {
    return dynamic_cast<const backend::pointer_type_impl*>(this);
}


const backend::record_type_impl * type_impl_common::try_as_record() const {
    return dynamic_cast<const backend::record_type_impl*>(this);
}


const backend::typedef_type_impl * type_impl_common::try_as_typedef() const {
    return dynamic_cast<const backend::typedef_type_impl*>(this);
}


const backend::mem_ptr_type_impl * type_impl_common::try_as_mem_ptr() const {
    return dynamic_cast<const backend::mem_ptr_type_impl*>(this);
}


const backend::type_impl * type_impl_common::untypedef() const {
    const backend::type_impl * type = this;
    while (true) {
        auto td_type = dynamic_cast<const backend::typedef_type_impl*>(type);
        if (!td_type) {
            return type;
        }

        type = td_type->base();
    }
}


}
