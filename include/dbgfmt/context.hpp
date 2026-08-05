
/// \file context.hpp
/// Contains definition of the context class.

#pragma once

#include "debug_object.hpp"
#include "formatter.hpp"
#include "format_options.hpp"
#include "format_error.hpp"
#include "backend/builtin_type_impl.hpp"
#include "backend/forward.hpp"
#include "backend/type_context_impl.hpp"
#include "backend/value_context_impl.hpp"
#include <list>
#include <locale>
#include <stack>


namespace dbgfmt {


/// Formatting context. Stores infomration about all available formatters
/// and current formatting state
class context {
public:
    /// Constructs formatting context with specified reference to
    /// type and value context implementation
    context(backend::type_context_impl & tctx, backend::value_context_impl & vctx);

    /// Destructor, destroys object
    virtual ~context() {}

    /// Formats value. If no_raw_data parameter is true then [Raw Data] field will not
    /// be displayed for result of formatting of root value.
    fmt_result_ref format(const value & val);

    /// Formats value with specified formatting options
    fmt_result_ref format(const value & val, const format_options & opts);

    /// Formats value without custom formatters
    fmt_result_ref format_raw(const value & val);

    /// Returns address size for target platform
    std::size_t address_size() const;

    /// Writes address to output stream
    void write_address(std::wostream & str, std::uint64_t addr) const;

    /// Tries find type with specified name. Returns invalid type if not found.
    value_type try_find_type(const std::string & name);

    /// Returns type with specified name. Throws exception if type not found.
    value_type find_type(const std::string & name);

    /// Makes builtin type of specified kind
    value_type bt_type(backend::builtin_type_impl::kind_t kind);

    /// Makes temporary value of specified type
    value make_temp_val(const value_type & type);

    /// Returns current format options
    const format_options & fmt_opts() const;

    /// Sets current format options
    void set_fmt_opts(const format_options & fopts) { fmt_opts_ = fopts; }

    /// Returns true if "Raw Data" child should be displayed
    bool show_raw_data() const { return fmt_opts().show_raw_data(); }

    /// Sets if "Raw Data" child should be displayed in value formatter with
    /// custom formatter
    void set_show_raw_data(bool v) {
        auto opts = fmt_opts();
        opts.set_show_raw_data(v);
        set_fmt_opts(opts);
    }

    /// Returns minimum hex size
    auto min_hex_size() const { return fmt_opts().min_hex_size(); }

    /// Sets minimum hex size
    void set_min_hex_size(size_t val) {
        auto opts = fmt_opts();
        opts.set_min_hex_size(val);
        set_fmt_opts(opts);
    }

    /// Returns "display hex" format option
    bool hex() const { return fmt_opts().hex(); }

    /// Sets "display hex" format options
    void set_hex(bool val) {
        auto opts = fmt_opts();
        opts.set_hex(val);
        set_fmt_opts(opts);
    }

    /// Returns "display pointer addresses" format option
    bool show_ptr_addr() const { return fmt_opts().show_ptr_addr(); }

    /// Sets "display pointer addresses" format option
    void set_show_ptr_addr(bool val) {
        auto opts = fmt_opts();
        opts.set_show_ptr_addr(val);
        set_fmt_opts(opts);
    }

    /// Tries find and returns debug object located at specified address.
    /// Returns invalid object if not found
    debug_object try_obj_at_addr(uint64_t addr) const {
        return debug_object{val_ctx_.obj_at_addr(addr)};
    }

    /// Finds and returns debug object located at specified address.
    /// Thows format_error if object not found
    debug_object obj_at_addr(uint64_t addr) const {
        auto res = try_obj_at_addr(addr);
        if (res) {
            return res;
        }

        std::ostringstream msg;
        msg << "can't find debug object located at address " << addr;
        throw format_error{msg.str()};
    }

private:
    /// Registers custom formatter
    template <typename T>
    void register_formatter();

    /// Registers builtin formatter
    template <typename T>
    void register_builtin_formatter();

    /// Tries format value using formatter from passed list of formatters
    fmt_result_ref try_format(const std::list<formatter_ref> & fmt_list,
                              const value & val,
                              bool disp_raw_data);


    backend::type_context_impl & type_ctx_;      ///< Reference to type context implementation
    backend::value_context_impl & val_ctx_;      ///< Reference to value context implementation

    /// List of custom formatters
    std::list<formatter_ref> custom_formatters_;

    /// List of builtin formatters
    std::list<formatter_ref> builtin_formatters_;

    std::size_t addr_size_;             ///< Address size for target platform
    format_options fmt_opts_;           ///< Format options

    /// Stack of additional format options
    std::stack<format_options> fmt_opts_stack_;
};


}


