

add_subdirectory(data)
add_subdirectory(backend)


# Formatters library

add_library(cxxdbg-dbg-fmt ${LIBRARY_TYPE}
            array_formatter.cpp
            array_formatter.hpp
            bool_formatter.hpp
            char_formatter.cpp
            char_formatter.hpp
            complex_formatter.cpp
            complex_formatter.hpp
            container_iterator_formatter.cpp
            container_iterator_formatter.hpp
            container_formatter.cpp
            container_formatter.hpp
            context.cpp
            enum_formatter.cpp
            enum_formatter.hpp
            error_formatter.hpp
            float_formatter.hpp
            functor_formatter.cpp
            functor_formatter.hpp
            fmt_result.cpp
            int_formatter.hpp
            lambda_formatter.cpp
            lambda_formatter.hpp
            log.hpp
            mem_fun_formatter.hpp
            mem_var_formatter.cpp
            mem_var_formatter.hpp
            old_mem_fun_formatter.hpp
            pointer_formatter.cpp
            pointer_formatter.hpp
            print.hpp
            proxy_fmt_result.hpp
            record_formatter.cpp
            record_formatter.hpp
            simple_formatter.hpp
            smart_ptr_formatter.cpp
            smart_ptr_formatter.hpp
            utils.cpp
            utils.hpp
            value.cpp
            value_impl_common.cpp
            value_conv_formatter.hpp
            vector_formatter.hpp

            stl/std_bind_result_formatter.cpp
            stl/std_bind_result_formatter.hpp
            stl/std_bitset_formatter.cpp
            stl/std_bitset_formatter.hpp
            stl/std_chrono_formatter.cpp
            stl/std_chrono_formatter.hpp
            stl/std_forward_list_formatter.cpp
            stl/std_forward_list_formatter.hpp
            stl/std_list_formatter.cpp
            stl/std_list_formatter.hpp
            stl/std_shared_ptr_formatter.cpp
            stl/std_shared_ptr_formatter.hpp
            stl/std_string_formatter.cpp
            stl/std_string_formatter.hpp
            stl/std_tuple_formatter.cpp
            stl/std_tuple_formatter.hpp
            stl/std_vector_formatter.cpp
            stl/std_vector_formatter.hpp

            stl/stdcpp/stdcpp_utils.cpp
            stl/stdcpp/stdcpp_utils.hpp
            stl/stdcpp/stdcpp_any_formatter.cpp
            stl/stdcpp/stdcpp_any_formatter.hpp
            stl/stdcpp/stdcpp_array_formatter.cpp
            stl/stdcpp/stdcpp_array_formatter.hpp
            stl/stdcpp/stdcpp_auto_ptr_formatter.cpp
            stl/stdcpp/stdcpp_auto_ptr_formatter.hpp
            stl/stdcpp/stdcpp_bind_result_formatter.hpp
            stl/stdcpp/stdcpp_bitset_formatter.hpp
            stl/stdcpp/stdcpp_complex_formatter.cpp
            stl/stdcpp/stdcpp_complex_formatter.hpp
            stl/stdcpp/stdcpp_cxx11_list_formatter.hpp
            stl/stdcpp/stdcpp_cxx11_string_formatter.cpp
            stl/stdcpp/stdcpp_cxx11_string_formatter.hpp
            stl/stdcpp/stdcpp_deque_formatter.cpp
            stl/stdcpp/stdcpp_deque_formatter.hpp
            stl/stdcpp/stdcpp_forward_list_formatter.hpp
            stl/stdcpp/stdcpp_function_formatter.cpp
            stl/stdcpp/stdcpp_function_formatter.hpp
            stl/stdcpp/stdcpp_hashtable_formatter.cpp
            stl/stdcpp/stdcpp_hashtable_formatter.hpp
            stl/stdcpp/stdcpp_list_formatter.hpp
            stl/stdcpp/stdcpp_optional_formatter.cpp
            stl/stdcpp/stdcpp_optional_formatter.hpp
            stl/stdcpp/stdcpp_placeholder_formatter.hpp
            stl/stdcpp/stdcpp_reference_wrapper_formatter.hpp
            stl/stdcpp/stdcpp_stack_formatter.cpp
            stl/stdcpp/stdcpp_stack_formatter.hpp
            stl/stdcpp/stdcpp_string_formatter.cpp
            stl/stdcpp/stdcpp_string_formatter.hpp
            stl/stdcpp/stdcpp_string_view_formatter.hpp
            stl/stdcpp/stdcpp_tree_formatter.cpp
            stl/stdcpp/stdcpp_tree_formatter.hpp
            stl/stdcpp/stdcpp_tuple_formatter.cpp
            stl/stdcpp/stdcpp_tuple_formatter.hpp
            stl/stdcpp/stdcpp_queue_formatter.cpp
            stl/stdcpp/stdcpp_queue_formatter.hpp
            stl/stdcpp/stdcpp_shared_ptr_formatter.hpp
            stl/stdcpp/stdcpp_unique_ptr_formatter.cpp
            stl/stdcpp/stdcpp_unique_ptr_formatter.hpp
            stl/stdcpp/stdcpp_valarray_formatter.cpp
            stl/stdcpp/stdcpp_valarray_formatter.hpp
            stl/stdcpp/stdcpp_variant_formatter.cpp
            stl/stdcpp/stdcpp_variant_formatter.hpp
            stl/stdcpp/stdcpp_vector_formatter.cpp
            stl/stdcpp/stdcpp_vector_formatter.hpp
            stl/stdcpp/stdcpp_regex_formatter.cpp
            stl/stdcpp/stdcpp_regex_formatter.hpp

            stl/libcxx/libcxx_vector_formatter.cpp
            stl/libcxx/libcxx_vector_formatter.hpp
            stl/libcxx/libcxx_string_formatter.cpp
            stl/libcxx/libcxx_string_formatter.hpp
            stl/libcxx/libcxx_tree_formatter.cpp
            stl/libcxx/libcxx_tree_formatter.hpp
            stl/libcxx/libcxx_deque_formatter.cpp
            stl/libcxx/libcxx_deque_formatter.hpp
            stl/libcxx/libcxx_hash_table_formatter.cpp
            stl/libcxx/libcxx_hash_table_formatter.hpp
            stl/libcxx/libcxx_valarray_formatter.cpp
            stl/libcxx/libcxx_valarray_formatter.hpp
            stl/libcxx/libcxx_array_formatter.cpp
            stl/libcxx/libcxx_array_formatter.hpp
            stl/libcxx/libcxx_bitset_formatter.hpp
            stl/libcxx/libcxx_list_formatter.hpp
            stl/libcxx/libcxx_smart_ptr_formatter.hpp
            stl/libcxx/libcxx_tuple_formatter.hpp
            stl/libcxx/libcxx_function_formatter.cpp
            stl/libcxx/libcxx_function_formatter.hpp
            stl/libcxx/libcxx_placeholder_formatter.hpp
            stl/libcxx/libcxx_ns_utils.cpp
            stl/libcxx/libcxx_ns_utils.hpp
            stl/libcxx/libcxx_regex_formatter.cpp
            stl/libcxx/libcxx_regex_formatter.hpp
           )

target_precompile_headers(cxxdbg-dbg-fmt PRIVATE import.hpp)

target_include_directories(cxxdbg-dbg-fmt PUBLIC "${DBGFMT_INCLUDE_DIR}")

target_link_libraries(cxxdbg-dbg-fmt PUBLIC cxxdbg-dbg-fmtcore cxxtn Boost::log)


# library that contains common implementation of dbgfmtcore interfaces
add_library(cxxdbg-dbg-fmtcommon ${LIBRARY_TYPE}
            type_impl_common.cpp)
target_include_directories(cxxdbg-dbg-fmtcommon PUBLIC "${DBGFMT_INCLUDE_DIR}")
target_link_libraries(cxxdbg-dbg-fmtcommon PUBLIC cxxdbg-dbg-fmtcore)


add_subdirectory(test)
