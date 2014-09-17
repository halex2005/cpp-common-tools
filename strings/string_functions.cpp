#include "string_functions.h"
#include <algorithm>
#include <array_size.h>
#include <cstring>
#include <boost/nowide/convert.hpp>
#include "config.in.h"

/// \defgroup strings strings
/// \ingroup common-tools
///
/// This group contains utility classes and functions work with strings.

/// \ingroup strings
/// \brief contains functions that works with strings
namespace strings
{
    /// \brief String copy
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied
    /// \param [in]  sourceSize    - copy at most chars count from source
    /// \return Number of characters actual copied to destination buffer
    size_t string_copy(char *buffer, size_t bufferMaxSize, const char *source, size_t sourceSize)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        strcpy(buffer, "");
        if (!source)
            return 0;
        strncat(buffer, source, std::min(bufferMaxSize - 1, sourceSize));
        return strlen(buffer);
    }

    /// \brief String copy
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied (must be null-terminated string)
    /// \return Number of characters actual copied to destination buffer
    size_t string_copy(char *buffer, size_t bufferMaxSize, const char *source)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        strcpy(buffer, "");
        if (!source)
            return 0;
        strncat(buffer, source, bufferMaxSize-1);
        return strlen(buffer);
    }

    /// \brief String copy with narrowing of source
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied
    /// \param [in]  sourceSize    - copy at most chars count from source
    /// \return Number of characters actual copied to destination buffer
    /// \note Destination buffer will be encoded with UTF-8 encoding.
    size_t string_copy(char *buffer, size_t bufferMaxSize, const wchar_t *source, size_t sourceSize)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        strcpy(buffer, "");
        if (!source)
            return 0;
        boost::nowide::narrow(buffer, bufferMaxSize, source, source + sourceSize);
        return strlen(buffer);
    }

    /// \brief String copy with narrowing of source
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied (must be null-terminated string)
    /// \return Number of characters actual copied to destination buffer
    /// \note Destination buffer will be encoded with UTF-8 encoding.
    size_t string_copy(char *buffer, size_t bufferMaxSize, const wchar_t *source)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        strcpy(buffer, "");
        if (!source)
            return 0;
        boost::nowide::narrow(buffer, bufferMaxSize, source);
        return strlen(buffer);
    }

    /// \brief String copy with widening of source
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied
    /// \param [in]  sourceSize    - copy at most chars count from source
    /// \return Number of characters actual copied to destination buffer
    /// \note It is expected that source string is valid UTF-8 encoded string.
    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const char *source, size_t sourceSize)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        wcscpy(buffer, L"");
        if (!source)
            return 0;
        boost::nowide::widen(buffer, bufferMaxSize, source, source + sourceSize);
        return wcslen(buffer);
    }

    /// \brief String copy with widening of source
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied (must be null-terminated string)
    /// \return Number of characters actual copied to destination buffer
    /// \note It is expected that source string is valid UTF-8 encoded string.
    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const char *source)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        wcscpy(buffer, L"");
        if (!source)
            return 0;
        boost::nowide::widen(buffer, bufferMaxSize, source);
        return wcslen(buffer);
    }

    /// \brief String copy
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied
    /// \param [in]  sourceSize    - copy at most chars count from source
    /// \return Number of characters actual copied to destination buffer
    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const wchar_t *source, size_t sourceSize)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        wcscpy(buffer, L"");
        if (!source)
            return 0;
        wcsncat(buffer, source, std::min(bufferMaxSize - 1, sourceSize));
        return wcslen(buffer);
    }

    /// \brief String copy
    /// \param [out] buffer        - destination buffer
    /// \param [in]  bufferMaxSize - destination buffer len
    /// \param [in]  source        - source string to be copied (must be null-terminated string)
    /// \return Number of characters actual copied to destination buffer
    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const wchar_t *source)
    {
        if (!buffer || !bufferMaxSize)
            return 0;
        wcscpy(buffer, L"");
        if (!source)
            return 0;
        wcsncat(buffer, source, bufferMaxSize-1);
        return wcslen(buffer);
    }
}

// buffer_to_string implementation
namespace strings
{
    namespace detail
    {

        inline char get_digit(unsigned char byte)
        {
            static char temp_table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
            return temp_table[byte & 0xf];
        }

        template <class char_type>
        size_t buffer_to_string_implementation(char_type *dest, size_t dest_len, const void *void_src, size_t src_len, char_type delimiter)
        {
            char_type *p;
            const uint8_t *src = static_cast<const uint8_t *>(void_src);
            bool big = false;

            if (!(dest && dest_len))
                return 0;
            dest[0] = 0;
            if (!(src && src_len))
                return 0;

            char_type too_big[] = { '.', '.', '.', '\0' };
            if (dest_len < ArraySize(too_big))
                return 0;

            if (delimiter)
            {
                if (src_len * 3 > dest_len)
                {
                    src_len = (dest_len - ArraySize(too_big)) / 3;
                    big = true;
                }

                p = dest;
                for (size_t i = 0; i < src_len; ++i)
                {
                    unsigned char byte = src[i];
                    *p++ = get_digit((byte & 0xf0) >> 4);
                    *p++ = get_digit(byte & 0x0f);
                    *p++ = delimiter;
                }
                if (0 != (p - dest))
                    --p;
            }
            else
            {
                if ((src_len*2+1) > dest_len)
                {
                    src_len = (dest_len - ArraySize(too_big)) / 2;
                    big = true;
                }

                p = dest;
                for (size_t i = 0; i < src_len; ++i)
                {
                    unsigned char byte = src[i];
                    *p++ = get_digit((byte & 0xf0) >> 4);
                    *p++ = get_digit(byte & 0x0f);
                }
            }

            if (big)
                p += string_copy(p, ArraySize(too_big), too_big);
            else
                *p = '\0';
            return size_t(p - dest);
        }
    }

    /// \brief Convert binary data to string representation as hex byte values
    /// \param [out] dest      - destination buffer
    /// \param [in]  dest_len  - destination buffer len
    /// \param [in]  src       - pointer to data
    /// \param [in]  src_len   - size of data
    /// \param [in]  delimiter - delimiter character between neighbor bytes in string (0 to turn off delimiter)
    /// \return Number of characters actual written to destination buffer
    ///
    /// Usage:
    ///
    /// ~~~{.c}
    /// char buffer[1024];
    /// size_t data = 0xEFBEADDE;
    ///
    /// // example: default delimiter is space
    /// buffer_to_string(buffer, ArraySize(buffer), &data, sizeof(data))
    /// CHECK(std::string("DE AD BE EF") == buffer);
    ///
    /// // example: change delimiter to dash
    /// buffer_to_string(buffer, ArraySize(buffer), &data, sizeof(data), '-')
    /// CHECK(std::string("DE AD BE EF") == buffer);
    ///
    /// // example: turn off delimiter character
    /// buffer_to_string(buffer, ArraySize(buffer), &data, sizeof(data), 0)
    /// CHECK(std::string("DEADBEEF") == buffer);
    /// ~~~
    size_t buffer_to_string(char *dest, size_t dest_len, const void *src, size_t src_len, char delimiter)
    {
        return detail::buffer_to_string_implementation(dest, dest_len, src, src_len, delimiter);
    }
    /// \overload
    size_t buffer_to_string(wchar_t *dest, size_t dest_len, const void *src, size_t src_len, wchar_t delimiter)
    {
        return detail::buffer_to_string_implementation(dest, dest_len, src, src_len, delimiter);
    }
}

#include <cstdio>

namespace strings
{
    /// \brief Formatted output conversion to string.
    ///
    /// This function differ from standard C99.
    ///
    /// The functions snprintf() and vsnprintf() do not write more than
    /// size bytes (including the terminating null byte ('\0')).
    /// If the output was truncated due to this limit then the return value
    /// is the actual number of characters (excluding the terminating null byte)
    /// which have been written to the final string.
    ///
    /// If output is zero length, function will return zero.
    ptrdiff_t snprintf(char *dest, size_t dest_len, const char *format, ...)
    {
#if HAVE_SNPRINTF == 1
        if (dest == nullptr || dest_len == 0)
            return 0;
        va_list ap;
        va_start(ap, format);
        int result = ::vsnprintf(dest, dest_len, format, ap);
        va_end(ap);
        return std::min(ptrdiff_t(result), ptrdiff_t(dest_len) - 1);
#else
#   if defined (_WIN32)
        int retval;
        va_list ap;
        va_start(ap, format);
        retval = _vsnprintf(dest, dest_len, format, ap);
        va_end(ap);

        dest[dest_len - 1] = 0;
        return retval > 0 ? retval : dest_len - 1;
#   endif
#endif
    }

    /// \brief Formatted output conversion to string.
    ///
    /// This function differ from standard C99.
    ///
    /// The functions snprintf() and vsnprintf() do not write more than
    /// size bytes (including the terminating null byte ('\0')).
    /// If the output was truncated due to this limit then the return value
    /// is the actual number of characters (excluding the terminating null byte)
    /// which have been written to the final string.
    ///
    /// If output is zero length, function will return zero.
    ptrdiff_t vsnprintf(char *dest, size_t dest_len, const char *format, va_list args)
    {
#if HAVE_SNPRINTF == 1
        if (dest == nullptr || dest_len == 0)
            return 0;
        int result = ::vsnprintf(dest, dest_len, format, args);
        return std::min(ptrdiff_t(result), ptrdiff_t(dest_len) - 1);
#else
#   if defined (_WIN32)
        int retval = _vsnprintf(dest, dest_len, format, args);
        dest[dest_len - 1] = 0;
        return retval > 0 ? retval : dest_len - 1;
#   endif
#endif
    }
}

