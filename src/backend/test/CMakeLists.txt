
# fmtcore test
add_executable(dbgfmtcoretest
               test.cpp
              )

target_link_libraries(dbgfmtcoretest PRIVATE cxxdbg-dbg-fmtcore)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(dbgfmtcoretest PRIVATE rt)
endif()

add_test(NAME dbgfmtcoretest COMMAND dbgfmtcoretest)
