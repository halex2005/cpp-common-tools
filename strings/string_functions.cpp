#include "string_functions.h"
#include <algorithm>
#include <array_size.h>
#include <cstring>
#include <boost/nowide/convert.hpp>

// string_copy implementation
namespace strings
{
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
        size_t buffer_to_string_implementation(char_type *dest, size_t dest_len, const void *void_src, size_t src_len)
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
                *p++ = char_type(' ');
            }
            if (0 != (p - dest))
                --p;
            if (big)
                p += string_copy(p, ArraySize(too_big), too_big);
            else
                *p = '\0';
            return size_t(p - dest);
        }
    }

    size_t buffer_to_string(char *dest, size_t dest_len, const void *src, size_t src_len)
    {
        return detail::buffer_to_string_implementation(dest, dest_len, src, src_len);
    }
    /// \overload
    size_t buffer_to_string(wchar_t *dest, size_t dest_len, const void *src, size_t src_len)
    {
        return detail::buffer_to_string_implementation(dest, dest_len, src, src_len);
    }
}

#if HAVE_SNPRINTF != 1

namespace strings
{
#if defined (_WIN32)

    ptrdiff_t snprintf(char *dest, size_t dest_len, const char *format, ...)
    {
        int retval;
        va_list ap;
        va_start(ap, format);
        retval = _vsnprintf(dest, dest_len, format, ap);
        va_end(ap);

        dest[dest_len - 1] = 0;
        return retval > 0 ? retval : dest_len - 1;
    }

    ptrdiff_t vsnprintf(char *dest, size_t dest_len, const char *format, va_list args)
    {
        int retval = _vsnprintf(dest, dest_len, format, args);
        dest[dest_len - 1] = 0;
        return retval > 0 ? retval : dest_len - 1;
    }

#endif
}

#endif
