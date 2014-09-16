#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include "config.in.h"

namespace strings
{
    size_t string_copy(char *buffer, size_t bufferMaxSize, const char *source, size_t sourceSize);
    size_t string_copy(char *buffer, size_t bufferMaxSize, const char *source);

    size_t string_copy(char *buffer, size_t bufferMaxSize, const wchar_t *source, size_t sourceSize);
    size_t string_copy(char *buffer, size_t bufferMaxSize, const wchar_t *source);

    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const char *source, size_t sourceSize);
    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const char *source);

    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const wchar_t *source, size_t sourceSize);
    size_t string_copy(wchar_t *buffer, size_t bufferMaxSize, const wchar_t *source);
}

namespace strings
{
    size_t buffer_to_string(char *dest, size_t dest_len, const void *src, size_t src_len, char delimiter = ' ');
    size_t buffer_to_string(wchar_t *dest, size_t dest_len, const void *src, size_t src_len, wchar_t delimiter = ' ');
}

namespace strings
{
    ptrdiff_t snprintf(char *dest, size_t dest_len, const char *format, ...);
    ptrdiff_t vsnprintf(char *dest, size_t dest_len, const char *format, va_list args);
}
