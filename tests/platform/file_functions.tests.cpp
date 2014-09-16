#include <array_size.h>
#include <catch/catch.hpp>
#include <platform/file_functions.h>
#include <boost/filesystem.hpp>
#include "config.in.h"

using namespace boost::filesystem;

SCENARIO("get_executable*", "[get_path][platform]")
{
    WHEN("path")
    {
        auto p = platform::get_executable_path();
        REQUIRE(path(p).filename() == TEST_COMMON_FILENAME);
    }

    WHEN("dir")
    {
        auto dir = platform::get_executable_dir();
        REQUIRE(exists(dir) == true);
    }
}

SCENARIO("get_full*", "[get_path][platform]")
{
    auto exePath = platform::get_executable_path();
    auto exeDir = platform::get_executable_dir();
    WHEN("get full path")
    {
        auto p = platform::get_full_path(TEST_COMMON_FILENAME);
        REQUIRE(exePath == p);
    }

    WHEN("get full dir")
    {
        auto p = platform::get_full_dir(TEST_COMMON_FILENAME);
        REQUIRE(exeDir == p);
    }
}

SCENARIO("find_file*", "[find_file][platform]")
{
    std::string result;
    const char *name = "file_functions.tests.cpp";
    const char *notExistantFile = "not-existant-file-name-here.txt";
    path searchPath = path(CMAKE_CURENT_SOURCE_DIRECTORY) / "platform";
    path expectedFilePath = searchPath / name;
    WHEN("file in directory exists")
    {
        THEN("find_file should return true")
        {
            CHECK(true == platform::find_file(name, searchPath.string().c_str(), result));
            CHECK(expectedFilePath == result);
        }

        THEN("find_file_recursive should return true")
        {
            CHECK(true == platform::find_file_recursive(name, CMAKE_CURENT_SOURCE_DIRECTORY, result));
            CHECK(expectedFilePath == result);
        }

        THEN("find_file with recursive=false should return true")
        {
            CHECK(true == platform::find_file(name, searchPath.string().c_str(), result, false));
            CHECK(expectedFilePath == result);
        }

        THEN("find_file with recursive=true should return true")
        {
            CHECK(true == platform::find_file(name, CMAKE_CURENT_SOURCE_DIRECTORY, result, true));
            CHECK(expectedFilePath == result);
        }
    }

    WHEN("file in directory doesn't exists")
    {
        THEN("find_file should return false")
        {
            CHECK(false == platform::find_file(notExistantFile, searchPath.string().c_str(), result));
        }

        THEN("find_file_recursive should return false")
        {
            CHECK(false == platform::find_file_recursive(notExistantFile, CMAKE_CURENT_SOURCE_DIRECTORY, result));
        }

        THEN("find_file with recursive=false should return true")
        {
            CHECK(false == platform::find_file(notExistantFile, searchPath.string().c_str(), result, false));
        }

        THEN("find_file with recursive=true should return true")
        {
            CHECK(false == platform::find_file(notExistantFile, searchPath.string().c_str(), result, true));
        }
    }

    WHEN("find_file non-recursive in multiple directories")
    {
        const char *directories[] = {
            CMAKE_CURENT_SOURCE_DIRECTORY,
            searchPath.string().c_str(),
        };
        THEN("should return true")
        {
            CHECK(true == platform::find_file(name, directories, ArraySize(directories), result));
            CHECK(expectedFilePath == result);
        }
        THEN("with recursive=false should return true")
        {
            CHECK(true == platform::find_file(name, directories, ArraySize(directories), result, false));
            CHECK(expectedFilePath == result);
        }
        THEN("should return false")
        {
            CHECK(false == platform::find_file(notExistantFile, directories, ArraySize(directories), result));
        }
        THEN("with recursive=false should return false")
        {
            CHECK(false == platform::find_file(notExistantFile, directories, ArraySize(directories), result, false));
        }
    }

    WHEN("find_file recursively in multiple directories")
    {
        const char *directories[] = {
            TEST_COMMON_DIRECTORY,
            CMAKE_CURENT_SOURCE_DIRECTORY,
        };
        THEN("should return true")
        {
            CHECK(true == platform::find_file_recursive(name, directories, ArraySize(directories), result));
            CHECK(expectedFilePath == result);
        }
        THEN("with recursive=true should return true")
        {
            CHECK(true == platform::find_file(name, directories, ArraySize(directories), result, true));
            CHECK(expectedFilePath == result);
        }
        THEN("should return false")
        {
            CHECK(false == platform::find_file_recursive(notExistantFile, directories, ArraySize(directories), result));
        }
        THEN("with recursive=true should return false")
        {
            CHECK(false == platform::find_file(notExistantFile, directories, ArraySize(directories), result, true));
        }
    }
}
