#ifndef __FORMATTER_HEADER_H__
#define __FORMATTER_HEADER_H__

#include <cstddef>
#include <cstdint>
#include <string>

namespace strings
{
    struct format_options
    {
        char formatString[32];
        size_t width;
        size_t precision;
    };


    size_t bool_format(char *buffer, size_t bufferSize, const format_options *format, void *value);
    size_t char_format(char *buffer, size_t bufferSize, const format_options *format, void *value);
    size_t wchar_t_format(char *buffer, size_t bufferSize, const format_options *format, void *value);
    size_t char_str_format(char *buffer, size_t bufferSize, const format_options *format, void *value);
    size_t wchar_t_str_format(char *buffer, size_t bufferSize, const format_options *format, void *value);
    size_t signed_integer_format(char *buffer, size_t bufferSize, const format_options *format, void *value);
    size_t unsigned_integer_format(char *buffer, size_t bufferSize, const format_options *format, void *value);
    size_t pointer_format(char *buffer, size_t bufferSize, const format_options *format, void *value);

    class formatter
    {
    public:
        typedef size_t (*format_value)(char *buffer, size_t bufferSize, const format_options *format, void *value);
        typedef void (*clean_value)(void *value);

        formatter(ptrdiff_t value)
        {
            _value = reinterpret_cast<void*>(value);
            _format = &signed_integer_format;
            _clean = nullptr;
        };

        formatter(size_t value)
        {
            _value = reinterpret_cast<void*>(value);
            _format = &unsigned_integer_format;
            _clean = nullptr;
        };

        formatter(void *value, format_value formatter, clean_value cleaner)
        {
            _value = value;
            _format = formatter;
            _clean = cleaner;
        }

        ~formatter()
        {
            if (_clean && _value)
                _clean(_value);
        }

        size_t format(char *buffer, size_t bufferSize, const format_options *format = nullptr)
        {
            if (_format)
                return _format(buffer, bufferSize, format, _value);
            return 0;
        }

    private:
        void *_value;
        format_value _format;
        clean_value _clean;
    };

    inline formatter get_formatter(int8_t value) { return formatter((ptrdiff_t)value); }
    inline formatter get_formatter(int16_t value) { return formatter((ptrdiff_t)value); }
    inline formatter get_formatter(int32_t value) { return formatter((ptrdiff_t)value); }
    inline formatter get_formatter(int64_t value) { return formatter((ptrdiff_t)value); }
    inline formatter get_formatter(uint8_t value) { return formatter((size_t)value); }
    inline formatter get_formatter(uint16_t value) { return formatter((size_t)value); }
    inline formatter get_formatter(uint32_t value) { return formatter((size_t)value); }
    inline formatter get_formatter(uint64_t value) { return formatter((size_t)value); }
    inline formatter get_formatter(bool value) {
        return formatter(reinterpret_cast<void*>(value), &bool_format, nullptr);
    }
    inline formatter get_formatter(char value) {
        return formatter(reinterpret_cast<void*>(value), &char_format, nullptr);
    }
    inline formatter get_formatter(wchar_t value) {
        return formatter(reinterpret_cast<void*>(value), &wchar_t_format, nullptr);
    }
    inline formatter get_formatter(char *value) {
        return formatter(reinterpret_cast<void*>(value), &char_str_format, nullptr);
    }
    inline formatter get_formatter(wchar_t *value) {
        return formatter(reinterpret_cast<void*>(value), &wchar_t_str_format, nullptr);
    }
    inline formatter get_formatter(const char *value) {
        return formatter(reinterpret_cast<void*>(const_cast<char*>(value)), &char_str_format, nullptr);
    }
    inline formatter get_formatter(const wchar_t *value) {
        return formatter(reinterpret_cast<void*>(const_cast<wchar_t*>(value)), &wchar_t_str_format, nullptr);
    }

    template<typename _CharT, typename _Traits, typename _Alloc>
    inline formatter get_formatter(const std::basic_string<_CharT, _Traits, _Alloc> &stringValue)
    {
        const _CharT *str = stringValue.c_str();
        return get_formatter(str);
    }

    template <class T>
    inline formatter get_formatter(T *value) {
        return formatter(reinterpret_cast<void*>(value), &pointer_format, nullptr);
    }
    template <class T>
    inline formatter get_formatter(const T *value) {
        return formatter(reinterpret_cast<void*>(const_cast<T*>(value)), &pointer_format, nullptr);
    }
}

#endif
