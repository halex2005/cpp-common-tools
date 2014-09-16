#ifndef __FILE_FUNCTIONS_HEADER_H__
#define __FILE_FUNCTIONS_HEADER_H__

#include <string>

namespace platform
{
    std::string get_executable_path();
    std::string get_executable_dir();
    std::string get_full_path(const std::string &originalName);
    std::string get_full_dir(const std::string &originalName);

    bool find_file(const char *fileName, const char *directory, std::string &result);
    bool find_file(const char *fileName, const char *directory, std::string &result, bool recursive);
    bool find_file_recursive(const char *fileName, const char *directory, std::string &result);
    bool find_file(const char *fileName, const char *directory[], size_t dirsCount, std::string &result);
    bool find_file_recursive(const char *fileName, const char *directory[], size_t dirsCount, std::string &result);
    bool find_file(const char *fileName, const char *directory[], size_t dirsCount, std::string &result, bool recursive);
    std::string find_file(const char *fileName, const char *directories[], size_t dirsCount, bool recursive=false);
}

#endif
