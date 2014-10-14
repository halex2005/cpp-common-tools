#include "formatter.h"
#include "string_functions.h"
#include <boost/nowide/convert.hpp>
#include <cstring>

namespace strings
{
    size_t bool_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        bool v = (bool)reinterpret_cast<size_t>(value);
        if (format == nullptr)
        {
            static const char* bool_values[] = {"false", "true"};
            const char *bool_str = bool_values[v ? 1 : 0];
            return string_copy(buffer, bufferSize, bool_str);
        }
        return str_printf(buffer, bufferSize, format->formatString, v);
    }

    size_t char_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        char v = (char)reinterpret_cast<ptrdiff_t>(value);
        if (format == nullptr)
        {
            if (bufferSize < 2)
                return 0;
            buffer[0] = v;
            buffer[1] = 0;
            return 1;
        }
        return str_printf(buffer, bufferSize, format->formatString, v);
    }

    size_t wchar_t_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        wchar_t v = (wchar_t)reinterpret_cast<ptrdiff_t>(value);
        if (format == nullptr)
        {
            boost::nowide::narrow(buffer, bufferSize, &v, &v + 1);
            return strlen(buffer);
        }
        return str_printf(buffer, bufferSize, format->formatString, v);
    }

    size_t char_str_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        const char *v = reinterpret_cast<const char *>(value);
        if (format == nullptr)
        {
            return string_copy(buffer, bufferSize, v);
        }
        return str_printf(buffer, bufferSize, format->formatString, v);
    }

    size_t wchar_t_str_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        const wchar_t *v = reinterpret_cast<const wchar_t *>(value);
        if (format == nullptr)
        {
            boost::nowide::narrow(buffer, bufferSize, v);
            return strlen(buffer);
        }
        return str_printf(buffer, bufferSize, format->formatString, v);
    }

    size_t signed_integer_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        static format_options defaultIntFormatOptions = { "%d", 0, 0 };
        if (format == nullptr) format = &defaultIntFormatOptions;

        ptrdiff_t v = reinterpret_cast<ptrdiff_t>(value);
        return str_printf(buffer, bufferSize, format->formatString, v);
    }

    size_t unsigned_integer_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        static format_options defaultIntFormatOptions = { "%d", 0, 0 };
        if (format == nullptr) format = &defaultIntFormatOptions;

        size_t v = reinterpret_cast<size_t>(value);
        return str_printf(buffer, bufferSize, format->formatString, v);
    }

    size_t pointer_format(char *buffer, size_t bufferSize, const format_options *format, void *value)
    {
        if (format == nullptr)
        {
            return str_printf(buffer, bufferSize, "%p", value);
        }
        return str_printf(buffer, bufferSize, format->formatString, value);
    }

}
