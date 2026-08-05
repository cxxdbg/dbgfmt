
/// \file libcxx_queue_formatter.hpp
/// Contains definition of the libc++ queue formatter.

#pragma once

#include "libcxx_ns_utils.hpp"
#include "../../proxy_fmt_result.hpp"
#include "../../value_conv_formatter.hpp"
#include "../../utils.hpp"


namespace dbgfmt {


/// Formatter for libc++ queue
class libcxx_queue_formatter: public value_conv_formatter {
public:
    /// Returns true if formatter accepts specified value.
    bool accepts(const value & v) const override {
        return is_libcxx_record(v, "queue");
    }

private:
    /// Converts value
    value convert(const value & val) const override {
        return val["c"];
    }

    /// Converts formatting result. Replaces type to std::queue
    fmt_result_ref convert(const value & val, const fmt_result_ref & res) const override {
        auto new_type = replace(res.type(), L"std::deque", L"std::queue");
        return make_fmt_result<ot_proxy_fmt_result>(res, new_type);
    }
};


}
