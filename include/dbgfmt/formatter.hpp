
/// \file formatter.hpp
/// Contains definition of custom formatter concept and related classes

#pragma once

#include "fmt_result.hpp"
#include "value.hpp"
#include <cassert>
#include <memory>


namespace dbgfmt {


class context;
class formatter;
class formatter_ref;


/// Interface for bridging formatter objects with external code
class formatter_bridge: virtual public ref_counted {
public:
    /// Sets pointer to formatting context
    virtual void br_set_fmt_ctx(context * ctx) = 0;

    /// Returns true if formatter accepts specified value
    virtual bool br_accepts(const value & v) const = 0;

    /// Formats value. Return formatting result
    virtual fmt_result_bridge * br_format(const value & v) = 0;
};


/// Reference wrapper around pointer to formatter bridge
class formatter_ref: public ref_counted_ref_base<formatter_bridge> {
    template <typename T, typename ... Args>
    friend formatter_ref make_formatter(Args && ... args);

public:
    /// Default constructor, makes invalid reference
    formatter_ref() = default;

    /// Sets formatting context
    void set_fmt_ctx(context * ctx) { get()->br_set_fmt_ctx(ctx); }

    /// Returns true if formatter accepts specified value
    bool accepts(const value & v) const { return get()->br_accepts(v); }

    /// Formats value. Returns reference to formatting result
    fmt_result_ref format(const value & v) const {
        return acquire_fmt_result(get()->br_format(v));
    }

private:
    /// Acquires pointer to formatter bridge. Does not change reference counter.
    explicit formatter_ref(formatter_bridge * fmt):
        ref_counted_ref_base<formatter_bridge>{fmt} {}
};


/// Creates formatter and assigns it to reference
template <typename T, typename ... Args>
formatter_ref make_formatter(Args && ... args) {
    auto fmt = std::make_unique<T>(std::forward<Args>(args)...);
    fmt->add_ref();
    return formatter_ref{fmt.release()};
}


/// Abstract custom value formatter
class formatter: virtual public formatter_bridge,
                 public ref_counted_impl {
public:
    /// Destructor, destroys object
    virtual ~formatter() = default;

    /// Returns true if formatter accepts specified value.
    /// May throw format_error exception.
    virtual bool accepts(const value & v) const = 0;

    /// Formats value. Returns formatting result
    virtual fmt_result_ref format(const value & val) = 0;


    // bridging interface implementation

    /// Retruns true if formatter accepts specified value
    bool br_accepts(const value & v) const override {
        return accepts(v);
    }

    /// Implementation of bridging format function
    fmt_result_bridge * br_format(const value & v) override {
        auto res = format(v);
        return res.do_return();
    }

    /// Sets pointer to formatting context
    void br_set_fmt_ctx(context * ctx) override {
        ctx_ = ctx;
        assert(ctx_ != nullptr);
    }

protected:
    /// Returns reference to current formatting context
    auto & ctx() const {
        assert(ctx_ != nullptr && "Formatting context was not initialized");
        return *ctx_;
    }

private:
    context * ctx_ = nullptr;               ///< Pointer to formatting context
};


/// Helper class for defining new formatters that
/// delegates all work to class nested from fmt_result.
template <typename FmtResult>
class formatter_t: public formatter {
public:
    /// Returns true if formatter accepts value. Calls FmtResult::accepts
    /// static function
    bool accepts(const value & v) const override {
        return FmtResult::accepts(v);
    }

    /// Formats value. Creates new instance of FmtResult and passes
    /// all parameters to its constructor.
    fmt_result_ref format(const value & val) override {
        return make_fmt_result<FmtResult>(val, &ctx());
    }
};


}


