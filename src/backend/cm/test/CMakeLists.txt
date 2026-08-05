

# FMTCM test
add_executable(dbgfmtcmtest
               type_context_impl_test.cpp
               test.cpp
              )

target_link_libraries(dbgfmtcmtest PRIVATE cxxdbg-dbg-fmtcm Boost::unit_test_framework)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(dbgfmtcmtest PRIVATE rt)
endif()

add_test(NAME dbgfmtcmtest COMMAND dbgfmtcmtest)

