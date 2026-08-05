
/// \file container_iterator_formatter.cpp
/// Contains implementation of the container_iterator_formatter class.

#include "container_iterator_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include <cassert>


namespace dbgfmt {


fmt_result_ref container_iterator_formatter::format(const value & it) {
    // getting and dereferencing pointer
    value ptr = get_value_pointer(it);
    value val = *ptr;

    // formatting value
    auto fval = ctx().format(val);

    // creating result that has single [ptr] child
    auto res = std::make_unique<fixed_fmt_result>(fval.val(),
                                                  it.wqual_type_name(),
                                                  source_position{},
                                                  it.type().def_pos());
    res->add_child(L"[ptr]", [this, ptr] {
        return ctx().format(ptr);
    });

    return make_ref(std::move(res));
}


}
