
# Formatters test
add_executable(dbgfmttest
               array_formatter_test.cpp
               array_value.hpp
               auto_memory.cpp
               auto_memory.hpp
               auto_memory_test.cpp
               builtin_value.hpp
               cm_builder.hpp
               container_formatter_test.cpp
               enum_value.hpp
               error_value.hpp
               fmt_result_test.cpp
               fmt_test_fixture.hpp
               lambda_formatter_test.cpp
               memory.cpp
               memory.hpp
               memory_test.cpp
               mem_fun_formatter_test.cpp
               mock.hpp
               pointer_formatter_test.cpp
               pointer_value.hpp
               record_value.hpp
               record_formatter_test.cpp
               ref_counted_test.cpp
               template_name_test.cpp
               simple_formatter_test.cpp
               test.cpp
               test.hpp
               test2.cpp
               test2.hpp
               test_context.cpp
               test_context.hpp
               value.hpp
               value_context_impl.cpp
               value_context_impl.hpp
               values.hpp
               vector_formatter_test.cpp
               util.hpp

               stl/std_vector_formatter_test.cpp
               stl/std_string_formatter_test.cpp
               stl/std_tree_formatter_test.cpp
               stl/std_deque_formatter_test.cpp
               stl/std_queue_formatter_test.cpp
               stl/std_stack_formatter_test.cpp
               stl/std_hashtable_formatter_test.cpp
               stl/std_valarray_formatter_test.cpp
               stl/std_array_formatter_test.cpp
               stl/std_bitset_formatter_test.cpp
               stl/std_list_formatter_test.cpp
               stl/std_forward_list_formatter_test.cpp
               stl/std_tuple_formatter_test.cpp
               stl/std_function_formatter_test.cpp
               stl/std_bind_result_formatter_test.cpp
               stl/std_chrono_test.cpp
               stl/std_regex_formatter_test.cpp

               stl/stdcpp/stdcpp_any_formatter_test.cpp
               stl/stdcpp/stdcpp_auto_ptr_test.cpp
               stl/stdcpp/stdcpp_bind.cpp
               stl/stdcpp/stdcpp_bind.hpp
               stl/stdcpp/stdcpp_complex_formatter_test.cpp
               stl/stdcpp/stdcpp_deque.cpp
               stl/stdcpp/stdcpp_deque.hpp
               stl/stdcpp/stdcpp_normal_iterator.cpp
               stl/stdcpp/stdcpp_normal_iterator.hpp
               stl/stdcpp/stdcpp_optional_formatter_test.cpp
               stl/stdcpp/stdcpp_placeholder_formatter_test.cpp
               stl/stdcpp/stdcpp_string.cpp
               stl/stdcpp/stdcpp_string.hpp
               stl/stdcpp/stdcpp_string_view_formatter_test.cpp
               stl/stdcpp/stdcpp_tuple.cpp
               stl/stdcpp/stdcpp_tuple.hpp
               stl/stdcpp/stdcpp_reference_wrapper_formatter_test.cpp
               stl/stdcpp/stdcpp_shared_ptr_test.cpp
               stl/stdcpp/stdcpp_unique_ptr_test.cpp
               stl/stdcpp/stdcpp_utils.cpp
               stl/stdcpp/stdcpp_utils.hpp
               stl/stdcpp/stdcpp_valarray.cpp
               stl/stdcpp/stdcpp_valarray.hpp
               stl/stdcpp/stdcpp_variant_formatter_test.cpp
               stl/stdcpp/stdcpp_vector.cpp

               stl/libcxx/libcxx_complex_formatter_test.cpp
               stl/libcxx/libcxx_deque.cpp
               stl/libcxx/libcxx_deque.hpp
               stl/libcxx/libcxx_smart_ptr_formatter_test.cpp
               stl/libcxx/libcxx_string.cpp
               stl/libcxx/libcxx_string.hpp
               stl/libcxx/libcxx_tuple.hpp
               stl/libcxx/libcxx_utils.cpp
               stl/libcxx/libcxx_utils.hpp
               stl/libcxx/libcxx_valarray.cpp
               stl/libcxx/libcxx_valarray.hpp
               stl/libcxx/libcxx_vector.cpp
               stl/libcxx/libcxx_vector.hpp
               stl/libcxx/libcxx_wrap_iter.cpp
               stl/libcxx/libcxx_wrap_iter.hpp
              )

target_precompile_headers(dbgfmttest PRIVATE import.hpp)
target_link_libraries(dbgfmttest PRIVATE cxxdbg-dbg-fmtcommon cxxdbg-dbg-fmt cxxdbg-dbg-fmtcm cxxdbg-dbg-fmtcore Boost::unit_test_framework)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(dbgfmttest PRIVATE rt)
endif()

add_test(NAME dbgfmttest COMMAND dbgfmttest)
