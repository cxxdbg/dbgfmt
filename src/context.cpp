
/// \file context.cpp
/// Contains implementation of context class.

#include "dbgfmt/context.hpp"
#include "array_formatter.hpp"
#include "bool_formatter.hpp"
#include "char_formatter.hpp"
#include "complex_formatter.hpp"
#include "enum_formatter.hpp"
#include "error_formatter.hpp"
#include "dbgfmt/fixed_fmt_result.hpp"
#include "float_formatter.hpp"
#include "functor_formatter.hpp"
#include "int_formatter.hpp"
#include "lambda_formatter.hpp"
#include "log.hpp"
#include "mem_fun_formatter.hpp"
#include "mem_var_formatter.hpp"
#include "nullptr_formatter.hpp"
#include "old_mem_fun_formatter.hpp"
#include "pointer_formatter.hpp"
#include "print.hpp"
#include "proxy_fmt_result.hpp"
#include "record_formatter.hpp"
#include "vector_formatter.hpp"
#include "dbgfmt/source_position.hpp"
#include "dbgfmt/format_error.hpp"
#include "stl/stdcpp/stdcpp_any_formatter.hpp"
#include "stl/stdcpp/stdcpp_array_formatter.hpp"
#include "stl/stdcpp/stdcpp_auto_ptr_formatter.hpp"
#include "stl/stdcpp/stdcpp_bind_result_formatter.hpp"
#include "stl/stdcpp/stdcpp_bitset_formatter.hpp"
#include "stl/stdcpp/stdcpp_complex_formatter.hpp"
#include "stl/stdcpp/stdcpp_cxx11_list_formatter.hpp"
#include "stl/stdcpp/stdcpp_cxx11_string_formatter.hpp"
#include "stl/stdcpp/stdcpp_deque_formatter.hpp"
#include "stl/stdcpp/stdcpp_forward_list_formatter.hpp"
#include "stl/stdcpp/stdcpp_function_formatter.hpp"
#include "stl/stdcpp/stdcpp_hashtable_formatter.hpp"
#include "stl/stdcpp/stdcpp_list_formatter.hpp"
#include "stl/stdcpp/stdcpp_queue_formatter.hpp"
#include "stl/stdcpp/stdcpp_optional_formatter.hpp"
#include "stl/stdcpp/stdcpp_placeholder_formatter.hpp"
#include "stl/stdcpp/stdcpp_reference_wrapper_formatter.hpp"
#include "stl/stdcpp/stdcpp_shared_ptr_formatter.hpp"
#include "stl/stdcpp/stdcpp_string_formatter.hpp"
#include "stl/stdcpp/stdcpp_string_view_formatter.hpp"
#include "stl/stdcpp/stdcpp_stack_formatter.hpp"
#include "stl/stdcpp/stdcpp_tree_formatter.hpp"
#include "stl/stdcpp/stdcpp_tuple_formatter.hpp"
#include "stl/stdcpp/stdcpp_unique_ptr_formatter.hpp"
#include "stl/stdcpp/stdcpp_valarray_formatter.hpp"
#include "stl/stdcpp/stdcpp_variant_formatter.hpp"
#include "stl/stdcpp/stdcpp_vector_formatter.hpp"
#include "stl/stdcpp/stdcpp_regex_formatter.hpp"
#include "stl/libcxx/libcxx_vector_formatter.hpp"
#include "stl/libcxx/libcxx_string_formatter.hpp"
#include "stl/libcxx/libcxx_tree_formatter.hpp"
#include "stl/libcxx/libcxx_deque_formatter.hpp"
#include "stl/libcxx/libcxx_queue_formatter.hpp"
#include "stl/libcxx/libcxx_stack_formatter.hpp"
#include "stl/libcxx/libcxx_hash_table_formatter.hpp"
#include "stl/libcxx/libcxx_valarray_formatter.hpp"
#include "stl/libcxx/libcxx_array_formatter.hpp"
#include "stl/libcxx/libcxx_bitset_formatter.hpp"
#include "stl/libcxx/libcxx_list_formatter.hpp"
#include "stl/libcxx/libcxx_forward_list_formatter.hpp"
#include "stl/libcxx/libcxx_string_view_formatter.hpp"
#include "stl/libcxx/libcxx_complex_formatter.hpp"
#include "stl/libcxx/libcxx_smart_ptr_formatter.hpp"
#include "stl/libcxx/libcxx_tuple_formatter.hpp"
#include "stl/libcxx/libcxx_function_formatter.hpp"
#include "stl/libcxx/libcxx_bind_result_formatter.hpp"
#include "stl/libcxx/libcxx_placeholder_formatter.hpp"
#include "stl/libcxx/libcxx_regex_formatter.hpp"
#include "stl/std_chrono_formatter.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>


namespace dbgfmt {


fmt_result_ref context::format(const value & val) {
    DBGFMT_LOG_DEBUG << "format " << val.type_name();

    // trying apply custom formatters
    auto cres = try_format(custom_formatters_, val, fmt_opts().show_raw_data());
    if (cres) {
        return std::move(cres);
    }

    // trying use builtin formatters
    return format_raw(val);    
}


fmt_result_ref context::format(const value & val, const format_options & opts) {
    fmt_opts_stack_.push(opts);
    auto res = format(val);
    fmt_opts_stack_.pop();
    return res;
}


fmt_result_ref context::format_raw(const value & val) {
    // Always disable [Raw Data] for builtin formatters
    auto bres = try_format(builtin_formatters_, val, false);
    if (bres) {
        return std::move(bres);
    }

    std::string type_name;
    std::wstring wtype_name;
    try {
        type_name = val.type_name();
        wtype_name = val.wtype_name();
    }
    catch(std::exception & eh) {
        type_name = "<error getting type name: ";
        type_name += eh.what();
        type_name += ">";
        wtype_name.assign(type_name.begin(), type_name.end());
    }

    // can't find formatter for a value
    DBGFMT_LOG_ERROR << "format_raw: can't find formatter for value of type '"
                        << type_name << "'";

    std::wostringstream msg;
    msg << L"<error: can't format value of type '" << wtype_name << L"'>";
    std::wstring smsg = msg.str();
    source_position p;
    return make_fmt_result<fixed_fmt_result>(smsg, L"<error>", p, p);
}


std::size_t context::address_size() const {
    return addr_size_;
}


void context::write_address(std::wostream & str, std::uint64_t addr) const {
    print_hex(str, addr, address_size(), fmt_opts().min_hex_size());
}


value_type context::try_find_type(const std::string & name) {
    return value_type{type_ctx_.find_type(name)};
}


value_type context::find_type(const std::string & name) {
    auto res = try_find_type(name);
    if (!res.is_valid()) {
        std::ostringstream msg;
        msg << "Can't find type with name '" << name << "'";
        throw format_error{msg.str()};
    }

    return res;
}


value_type context::bt_type(backend::builtin_type_impl::kind_t kind) {
    return value_type{type_ctx_.bt_type(kind)};   
}


value context::make_temp_val(const value_type & type) {
    return value{val_ctx_.make_temp_val(type.impl())};
}


const format_options & context::fmt_opts() const {
    if (!fmt_opts_stack_.empty()) {
        return fmt_opts_stack_.top();
    }

    return fmt_opts_;
}


context::context(backend::type_context_impl & tctx, backend::value_context_impl & vctx):
type_ctx_{tctx},
val_ctx_{vctx},
addr_size_(8) {
    // lambda formatter
    register_formatter<lambda_formatter>();

    // formatter of pointer to member function in old representation
    register_formatter<old_mem_fun_formatter>();

    // stdc++ containers
    register_formatter<stdcpp_array_formatter>();
    register_formatter<stdcpp_complex_formatter>();
    register_formatter<stdcpp_deque_formatter>();
    register_formatter<stdcpp_deque_iterator_formatter>();
    register_formatter<stdcpp_forward_list_formatter>();
    register_formatter<stdcpp_forward_list_iterator_formatter>();
    register_formatter<stdcpp_list_formatter>();
    register_formatter<stdcpp_list_iterator_formatter>();
    register_formatter<stdcpp_cxx11_list_formatter>();
    register_formatter<stdcpp_queue_formatter>();
    register_formatter<stdcpp_stack_formatter>();
    register_formatter<stdcpp_string_formatter>();
    register_formatter<stdcpp_cxx11_string_formatter>();
    register_formatter<stdcpp_string_view_formatter>();
    register_formatter<stdcpp_tuple_formatter>();
    register_formatter<stdcpp_vector_formatter>();
    register_formatter<stdcpp_vector_iterator_formatter>();
    register_formatter<stdcpp_tree_formatter>();
    register_formatter<stdcpp_tree_iterator_formatter>();
    register_formatter<stdcpp_hashtable_formatter>();
    register_formatter<stdcpp_hashtable_iterator_formatter>();
    register_formatter<stdcpp_bitset_formatter>();
    register_formatter<stdcpp_valarray_formatter>();
    register_formatter<stdcpp_slice_formatter>();
    register_formatter<stdcpp_slice_array_formatter>();

    // stdc++ smart pointers
    register_formatter<stdcpp_auto_ptr_formatter>();
    register_formatter<stdcpp_unique_ptr_formatter>();
    register_formatter<stdcpp_shared_ptr_formatter>();

    // functional formatters
    register_formatter<stdcpp_function_formatter>();
    register_formatter<stdcpp_placeholder_formatter>();
    register_formatter<stdcpp_bind_result_formatter>();
    register_formatter<stdcpp_reference_wrapper_formatter>();

    // other formatters
    register_formatter<stdcpp_any_formatter>();
    register_formatter<stdcpp_variant_formatter>();
    register_formatter<stdcpp_optional_formatter>();

    // regex formatters
    register_formatter<stdcpp_sub_match_formatter>();
    register_formatter<stdcpp_match_results_formatter>();

    // builtin formatters
    register_builtin_formatter<error_formatter>();
    register_builtin_formatter<bool_formatter>();
    register_builtin_formatter<int_formatter>();
    register_builtin_formatter<uint_formatter>();
    register_builtin_formatter<float_formatter>();
    register_builtin_formatter<array_formatter>();
    register_builtin_formatter<vector_formatter>();
    register_builtin_formatter<char_formatter>();
    register_builtin_formatter<wchar_formatter>();
    register_builtin_formatter<pointer_formatter>();
    register_builtin_formatter<record_formatter>();
    register_builtin_formatter<functor_formatter>();
    register_builtin_formatter<float_complex_formatter>();
    register_builtin_formatter<int_complex_formatter>();
    register_builtin_formatter<enum_formatter>();
    register_builtin_formatter<mem_fun_formatter>();
    register_builtin_formatter<mem_var_formatter>();
    register_builtin_formatter<nullptr_formatter>();

    // libc++ formatters
    register_formatter<libcxx_vector_formatter>();
    register_formatter<libcxx_vector_iterator_formatter>();
    register_formatter<libcxx_string_formatter>();
    register_formatter<libcxx_tree_formatter>();
    register_formatter<libcxx_tree_iterator_formatter>();
    register_formatter<libcxx_map_iterator_formatter>();
    register_formatter<libcxx_deque_formatter>();
    register_formatter<libcxx_deque_iterator_formatter>();
    register_formatter<libcxx_queue_formatter>();
    register_formatter<libcxx_stack_formatter>();
    register_formatter<libcxx_hash_table_formatter>();
    register_formatter<libcxx_hash_table_iterator_formatter>();
    register_formatter<libcxx_hash_map_iterator_formatter>();
    register_formatter<libcxx_valarray_formatter>();
    register_formatter<libcxx_slice_formatter>();
    register_formatter<libcxx_slice_array_formatter>();
    register_formatter<libcxx_array_formatter>();
    register_formatter<libcxx_bitset_formatter>();
    register_formatter<libcxx_list_formatter>();
    register_formatter<libcxx_list_iterator_formatter>();
    register_formatter<libcxx_forward_list_formatter>();
    register_formatter<libcxx_forward_list_iterator_formatter>();
    register_formatter<libcxx_string_view_formatter>();
    register_formatter<libcxx_complex_formatter>();
    register_formatter<libcxx_smart_ptr_formatter>();
    register_formatter<libcxx_shared_ptr_formatter>();
    register_formatter<libcxx_tuple_formatter>();
    register_formatter<libcxx_function_formatter>();
    register_formatter<libcxx_bind_result_formatter>();
    register_formatter<libcxx_placeholder_formatter>();
    register_formatter<libcxx_sub_match_formatter>();
    register_formatter<libcxx_match_results_formatter>();

    // common STL formatters
    register_formatter<std_chrono_duration_formatter>();
    register_formatter<std_chrono_time_point_formatter>();
}


template <typename T>
void context::register_formatter() {
    auto fmt = make_formatter<T>();
    fmt.set_fmt_ctx(this);
    custom_formatters_.push_back(fmt);
}


template <typename T>
void context::register_builtin_formatter() {
    auto fmt = make_formatter<T>();
    fmt.set_fmt_ctx(this);
    builtin_formatters_.push_back(fmt);
}


fmt_result_ref context::try_format(const std::list<formatter_ref> & fmt_list,
                                   const value & val,
                                   bool disp_raw_data) {

    // trying apply custom formatters
    for (const auto & fmt: fmt_list) {
        // checking if formatter accepts value and catching format_error
        // exception which may occur inside accepts method
        try {
            if (!fmt.accepts(val)) {
                continue;
            }
        }
        catch (format_error &) {
            // skip formatter on error in accepts method
            continue;
        }

        // formatting value and reporting format errors

        fmt_result_ref fval;

        try {
            // formatting value
            auto begin_time = std::chrono::steady_clock::now();
            fval = fmt.format(val);
            auto end_time = std::chrono::steady_clock::now();

            // checking that formatter returned valid result
            if (!fval) {
                throw format_error{"formatter returned invalid result"};
            }

            // calculating and saving format time
            auto fmt_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time);
            fval->set_format_time(fmt_time.count());

            // setting value address if not set
            if (fval.address() == UINT64_MAX && val.has_address()) {
                fval.set_address(val.address());
            }
        }
        catch (format_error & err) {
            // trying get type name to display in error value
            std::wstring wtype_name;
            try {
                wtype_name = val.wtype_name();
            }
            catch(std::exception & eh) {
                std::string err = eh.what();
                wtype_name = L"<error getting type name: ";
                wtype_name.append(err.begin(), err.end());
                wtype_name += L">";
            }

            std::wostringstream msg;
            msg << "<error: " << err.what() << ">";
            std::wstring smsg = msg.str();

            source_position p;
            fval = make_fmt_result<fixed_fmt_result>(smsg, wtype_name, p, p);
        }

        // wrapping formatted value with [Raw Data] decorator if enabled
        if (disp_raw_data) {
            auto proxy = std::make_unique<proxy_fmt_result>(fval);
            proxy->add_end_child(L"[Raw Data]", [this, val] {
                return format_raw(val);
            });
            fval = make_ref(std::move(proxy));
        }

        return fmt_result_ref{std::move(fval)};
    }

    return {};
}


}
