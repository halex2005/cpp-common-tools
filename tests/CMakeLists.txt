cmake_minimum_required(VERSION 2.8)

set (strings_tests
    strings/strings_headers.tests.cpp

    strings/formatter.tests.cpp
    strings/string_functions.tests.cpp
)
source_group(strings FILES ${strings_tests})

set (platform_tests
    platform/platform_headers.tests.cpp

    platform/file_functions.tests.cpp
    platform/thread_functions.tests.cpp
)
source_group(platform FILES ${platform_tests})

include_directories(.)

add_executable(test_common_tools main.cpp ${strings_tests} ${platform_tests})
target_link_libraries(test_common_tools ${Boost_LIBRARIES} common-tools)

get_property(TEST_COMMON_LOCATION TARGET test_common_tools PROPERTY LOCATION)
get_filename_component(TEST_COMMON_FILENAME ${TEST_COMMON_LOCATION} NAME)
get_filename_component(TEST_COMMON_DIRECTORY ${TEST_COMMON_LOCATION} PATH)
configure_file(platform/config.in ${CMAKE_CURRENT_SOURCE_DIR}/platform/config.in.h)