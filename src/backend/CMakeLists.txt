

# Formatters core library
add_library(cxxdbg-dbg-fmtcore ${LIBRARY_TYPE}
            format_error.cpp
           )

target_include_directories(cxxdbg-dbg-fmtcore PUBLIC "${DBGFMT_INCLUDE_DIR}")

target_link_libraries(cxxdbg-dbg-fmtcore cxxdbg-data)

add_subdirectory(test)
add_subdirectory(cm)
