

# CM formatters library
add_library(cxxdbg-dbg-fmtcm ${LIBRARY_TYPE}
            array_type_impl.hpp
            builtin_type_impl.hpp
            debug_object_impl.cpp
            debug_object_impl.hpp
            enum_type_impl.hpp
            fmtcm.hpp
            function_type_impl.hpp
            mem_ptr_type_impl.hpp
            pointer_type_impl.cpp
            pointer_type_impl.hpp
            record_type_impl.hpp
            symbol_impl.hpp
            type_context_impl.cpp
            type_context_impl.hpp
            type_impl.cpp
            type_impl.hpp
            typedef_type_impl.hpp
            value_context_impl.cpp
            value_context_impl.hpp
           )

target_precompile_headers(cxxdbg-dbg-fmtcm PRIVATE import.hpp)

target_include_directories(cxxdbg-dbg-fmtcm PUBLIC "${DBGFMT_INCLUDE_DIR}")

target_link_libraries(cxxdbg-dbg-fmtcm PUBLIC cxxdbg-dbg-fmtcore cxxdbg-dbg-fmtcommon cm cm-cxx cxxtn)

add_subdirectory(test)
