cmake_minimum_required(VERSION 2.8)

set (strings_tests
    strings/formatter.tests.cpp
    strings/string_functions.tests.cpp
)
source_group(strings FILES ${strings_tests})

include_directories(.)

add_executable(test_common_tools main.cpp ${strings_tests})
target_link_libraries(test_common_tools ${Boost_LIBRARIES} common-tools)
