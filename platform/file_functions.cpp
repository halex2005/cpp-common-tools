#include "file_functions.h"
#include "platform.h"
#include <array_size.h>
#include <boost/nowide/convert.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace platform
{
    std::string get_executable_path()
    {
        using namespace boost::filesystem;
#if defined(PLATFORM_WIN32)
        wchar_t fileName[1024];
        DWORD len = ::GetModuleFileNameW(nullptr, fileName, ArraySize(fileName));
        return boost::nowide::narrow(std::wstring(fileName, len));
#elif defined(PLATFORM_LINUX)
        char pBuf[1024];
        int bytes = std::min(size_t(readlink("/proc/self/exe", pBuf, ArraySize(pBuf))), ArraySize(pBuf) - 1);
        if (bytes >= 0)
            pBuf[bytes] = '\0';
        return absolute(path(pBuf)).string();
#else
        return "";
#endif
    }

    std::string get_executable_dir()
    {
        using namespace boost::filesystem;
        path processPath = get_executable_path();
        return processPath.remove_filename().string();
    }

    std::string get_full_path(const std::string &originalName)
    {
        using namespace boost::filesystem;
        path p(originalName);
        return absolute(p).string();
    }

    std::string get_full_dir(const std::string &originalName)
    {
        using namespace boost::filesystem;
        path p(originalName);
        return absolute(p).remove_filename().string();
    }
}

// find_file implementation
namespace platform
{
    bool find_file(const char *fileName, const char *directory, std::string &result)
    {
        using namespace boost::filesystem;
        if (directory == nullptr || fileName == nullptr) return false;
        if (!is_directory(directory))
            return false;

        directory_iterator end;
        for (auto iter = directory_iterator(directory); iter != end; ++iter)
        {
            const path &p = iter->path();
            if (is_regular_file(p) && p.filename() == fileName)
            {
                result = p.string();
                return true;
            }
        }
        return false;
    }

    bool find_file_recursive(const char *fileName, const char *directory, std::string &result)
    {
        using namespace boost::filesystem;
        if (directory == nullptr || fileName == nullptr) return false;
        if (!is_directory(directory))
            return false;

        typedef std::stack<path> stack_type;
        stack_type directories;
        directories.push(directory);

        directory_iterator end;
        while (!directories.empty())
        {
            path nextDirectory = std::move(directories.top());
            directories.pop();

            for (auto iter = directory_iterator(nextDirectory); iter != end; ++iter)
            {
                const path &p = iter->path();
                if (is_directory(p))
                {
                    directories.push(p);
                    continue;
                }
                if (is_regular_file(p) && p.filename() == fileName)
                {
                    result = p.string();
                    return true;
                }
            }
        }
        return false;
    }

    bool find_file(const char *fileName, const char *directory, std::string &result, bool recursive)
    {
        return recursive
            ? find_file_recursive(fileName, directory, result)
            : find_file(fileName, directory, result);
    }

    bool find_file(const char *fileName, const char *directory[], size_t dirsCount, std::string &result)
    {
        for (size_t i = 0; i < dirsCount; ++i)
        {
            if (find_file(fileName, directory[i], result))
                return true;
        }
        return false;
    }

    bool find_file_recursive(const char *fileName, const char *directory[], size_t dirsCount, std::string &result)
    {
        for (size_t i = 0; i < dirsCount; ++i)
        {
            if (find_file_recursive(fileName, directory[i], result))
                return true;
        }
        return false;
    }

    bool find_file(const char *fileName, const char *directory[], size_t dirsCount, std::string &result, bool recursive)
    {
        return recursive
            ? find_file_recursive(fileName, directory, dirsCount, result)
            : find_file(fileName, directory, dirsCount, result);
    }

    std::string find_file(const char *fileName, const char *directories[], size_t dirsCount, bool recursive)
    {
        std::string result;
        if (!find_file(fileName, directories, dirsCount, result, recursive))
            throw std::runtime_error("file not found in specified locations");
        return result;
    }
}
