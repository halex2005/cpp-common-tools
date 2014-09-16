CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(common-tools)

find_package(Boost 1.44 COMPONENTS log filesystem system)
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
)
set(string_sources
    strings/formatter.h
    strings/formatter.cpp
    strings/string_functions.h
    strings/string_functions.cpp
    strings/string_template.h
    strings/string_template.cpp
)

add_definitions(-std=c++11)
add_definitions(-fpermissive)
add_library(common-tools ${SOURCES} ${string_sources})
source_group(strings FILES ${string_sources} )

include_directories(.)
add_subdirectory(tests)

include(CheckFunctionExists)
include (CheckIncludeFiles)
CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_INCLUDE_FILES(magic.h HAVE_MAGIC_H)
configure_file(config.in ${CMAKE_CURRENT_SOURCE_DIR}/config.in.h)
