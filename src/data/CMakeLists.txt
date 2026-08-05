
# Data reader / write library

add_library(cxxdbg-data ${LIBRARY_TYPE}
            accessor.cpp
            error_storage.hpp
           )

target_include_directories(cxxdbg-data PUBLIC "${DBGFMT_INCLUDE_DIR}")

target_link_libraries(cxxdbg-data PUBLIC Boost::boost)

add_subdirectory(test)
