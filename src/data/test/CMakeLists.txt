
# Data library test

add_executable(datatest
               accessor_test.cpp
               test.cpp
              )

target_link_libraries(datatest PRIVATE cxxdbg-data Boost::unit_test_framework)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(datatest PRIVATE rt)
endif()

add_test(NAME datatest COMMAND datatest)
