#include "file_functions.h"
#include "platform.h"
#include <array_size.h>
#include <boost/nowide/convert.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace platform
{
    /// Get full path to current executable file
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

    /// Get directory which contains current executable file
    std::string get_executable_dir()
    {
        using namespace boost::filesystem;
        path processPath = get_executable_path();
        return processPath.remove_filename().string();
    }

    /// Get executable filename
    std::string get_executable_name()
    {
        using namespace boost::filesystem;
        path processPath = get_executable_path();
        return processPath.filename().string();
    }

    /// \brief Get absolute path to specified file
    /// \param originalName - file name which will be resolved to absolute path
    std::string get_full_path(const std::string &originalName)
    {
        using namespace boost::filesystem;
        path p(originalName);
        return absolute(p).string();
    }

    /// \brief Get directory which contains specified file
    /// \param originalName - file name which will be resolved to absolute dir
    std::string get_full_dir(const std::string &originalName)
    {
        using namespace boost::filesystem;
        path p(originalName);
        return absolute(p).remove_filename().string();
    }
}

// find_file implementations
namespace platform
{
    /// \brief Search file in specified directory.
    /// \param [in]  fileName  - file name to search
    /// \param [in]  directory - directory to search
    /// \param [out] result    - full path if file was found
    /// \return
    ///     - true - if file found, full path to file is written to result
    ///     - false - otherwise
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

    /// \brief Search file in specified directory recursively.
    /// \param [in]  fileName  - file name to search
    /// \param [in]  directory - directory to search
    /// \param [out] result    - full path if file was found
    /// \return
    ///     - true - if file found, full path to file is written to result
    ///     - false - otherwise
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

    /// \brief Search file in specified directory.
    /// \param [in]  fileName  - file name to search
    /// \param [in]  directory - directory to search
    /// \param [out] result    - full path if file was found
    /// \param [in]  recursive - flag to search file recursive
    /// \return
    ///     - true - if file found, full path to file is written to result
    ///     - false - otherwise
    bool find_file(const char *fileName, const char *directory, std::string &result, bool recursive)
    {
        return recursive
            ? find_file_recursive(fileName, directory, result)
            : find_file(fileName, directory, result);
    }

    /// \brief Search file in multiple directories.
    /// \param [in]  fileName    - file name to search
    /// \param [in]  directories - directories to search
    /// \param [in]  dirsCount   - count of directories in directories
    /// \param [out] result      - full path if file was found
    /// \param [in]  recursive - flag to search file recursive
    /// \return
    ///     - true - if file found, full path to file is written to result
    ///     - false - otherwise
    bool find_file(const char *fileName, const char *directories[], size_t dirsCount, std::string &result, bool recursive)
    {
        for (size_t i = 0; i < dirsCount; ++i)
        {
            if (find_file(fileName, directories[i], result, recursive))
                return true;
        }
        return false;
    }

    /// \brief Search file in multiple directories.
    /// \param [in]  fileName    - file name to search
    /// \param [in]  directories - directories to search
    /// \param [in]  dirsCount   - count of directories in directories
    /// \param [in]  recursive - flag to search file recursive
    /// \return
    ///     - if file found, full path to file will be returned
    ///     - otherwise std::runtime_error will be throwed
    std::string find_file(const char *fileName, const char *directories[], size_t dirsCount, bool recursive)
    {
        std::string result;
        if (!find_file(fileName, directories, dirsCount, result, recursive))
            throw std::runtime_error("file not found in specified locations");
        return result;
    }
}
