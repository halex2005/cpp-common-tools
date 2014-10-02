CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(common-tools)

find_package(Boost 1.44 COMPONENTS filesystem system)
if (NOT Boost_FOUND)
    message(SEND_ERROR "boost components not found")
    return()
else()
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

set(SOURCES
    config.in
    array_size.h
    platform.h
    strings.h
)
set(string_sources
    strings/formatter.h
    strings/formatter.cpp
    strings/string_functions.h
    strings/string_functions.cpp
    strings/string_template.h
    strings/string_template.cpp
)
source_group(strings FILES ${string_sources} )

set(platform_sources
    platform/file_functions.h
    platform/file_functions.cpp
    platform/handle.hpp
    platform/performance_counter.h
    platform/platform.h
    platform/thread_functions.h
    platform/thread_functions.cpp
)
source_group(platform FILES ${platform_sources})

add_definitions(-std=c++11)
add_definitions(-fpermissive)
add_library(common-tools
    ${SOURCES}
    ${platform_sources}
    ${string_sources}
)

include_directories(.)
add_subdirectory(tests)

include(CheckFunctionExists)
include (CheckIncludeFiles)
CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_INCLUDE_FILES(magic.h HAVE_MAGIC_H)
configure_file(config.in ${CMAKE_CURRENT_SOURCE_DIR}/config.in.h)

# doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/doxyfile.in ${CMAKE_BINARY_DIR}/doxyfile @ONLY)
    add_custom_target(common-tools-doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()
