
/// \file container_formatter.cpp
/// Contains implementation of container_formatter class.

#include "container_formatter.hpp"
#include "dbgfmt/context.hpp"
#include "dbgfmt/fmt_result.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "proxy_fmt_result.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include <cassert>
#include <sstream>


namespace dbgfmt {


const std::size_t container_formatter::max_container_size;


named_fmt_result container_fmt_result::child_at(size_t idx) const {
    std::wostringstream name;
    name << L"[" << idx << L"]";
    try {
        auto res = ctx_.format(fmt_.element(val_, idx));
        return named_fmt_result{name.str(), {}, res};
    }
    catch(std::exception & err) {
        return named_fmt_result{name.str(), {}, make_error_fmt_result(err.what())};
    }
}


fmt_result_ref container_formatter::format(const value & val) {
    // getting container size
    std::size_t sz = size(val);

    // checking size
    check_size(sz);

    // formatting value
    std::wstring val_str = format_value(val);

    // creating result for container elements
    auto res = make_fmt_result<container_fmt_result>(ctx(),
                                                     *this,
                                                     val,
                                                     val_str,
                                                     sz,
                                                     disp_type_name(val));

    // transforming result in inherited classes
    res = transform_result(val, res);

    // creating proxy for displaying capacity
    if (has_capacity()) {
        auto cap = capacity(val);
        auto capacity_proxy = std::make_unique<proxy_fmt_result>(res);
        capacity_proxy->add_begin_child(L"[capacity]", [cap] {
            std::wostringstream str;
            str << cap;
            return make_fmt_result<fixed_fmt_result>(str.str(),
                                                     L"std::size_t",
                                                     source_position{},
                                                     source_position{});
        });

        res = make_ref(std::move(capacity_proxy));
    }

    // creating proxy for displaying size
    if (display_size()) {
        res = make_size_proxy(res, sz);
    }

    return res;
}

#if 0

void container_formatter::do_format(const value & val,
                                    std::wstring & val_str,
                                    std::wstring & hval_str,
                                    std::size_t & nchilds,
                                    std::wstring & type,
                                    context * gfmt) const {
    // getting container size
    std::size_t sz = size(val);

    // checking size
    check_size(sz);

    // formatting value
    format_value(val, val_str, hval_str, gfmt);

    // calculating number of childs
    nchilds = static_cast<std::size_t>(sz) + 1;
    if (has_capacity()) {
        ++nchilds;
    }
}


named_fmt_result container_formatter::child(const value & val,
                                            std::size_t index,
                                            context * gfmt) const {
    if (index == 0) {
        // size of container
        std::wostringstream str;
        str << size(val);
        return make_named_fmt_result<fixed_fmt_result>(L"[size]", str.str(), L"std::size_t");
    } else if (index == 1 && has_capacity()) {
        // vector capacity
        std::wostringstream str;
        str << capacity(val);
        return make_named_fmt_result<fixed_fmt_result>(L"[capacity]", str.str(), L"std::size_t");
    } else {
        // container element
        std::size_t real_index = index - 1;

        if (has_capacity()) {
            -- real_index;
        }

        std::wostringstream name;
        name << L"[" << real_index << L"]";

        return named_fmt_result{name.str(), gfmt->format(element(val, real_index))};
    }
}

#endif


void container_formatter::check_size(std::size_t sz) {
    if (sz > max_container_size) {
        std::ostringstream msg;
        msg << "container is very large (size = " << sz << ")";
        throw dbgfmt::format_error{msg.str()};
    }
}


fmt_result_ref container_formatter::make_size_proxy(fmt_result_ref res, size_t sz) {
    auto size_proxy = std::make_unique<proxy_fmt_result>(res);
    size_proxy->add_begin_child(L"[size]", [sz] {
        std::wostringstream str;
        str << sz;
        return make_fmt_result<fixed_fmt_result>(str.str(),
                                                 L"std::size_t",
                                                 source_position{},
                                                 source_position{});
    });

    return make_ref(std::move(size_proxy));
}


bool container_formatter::has_capacity() const {
    return false;
}


std::size_t container_formatter::capacity(const value &) const {
    assert(false && "Can't get container capacity");
    return 0;
}


std::wstring container_formatter::disp_type_name(const value & val) const {
    return val.wqual_type_name();
}


std::wstring container_formatter::format_value(const value & val) const {

    std::wostringstream str;
    str << L"[size = " << size(val);

    if (has_capacity()) {
        str << L", capacity = " << capacity(val);
    }

    str << L"]";

    return str.str();
}


}
