#ifndef __COMMON_SINKS_HEADER_H__
#define __COMMON_SINKS_HEADER_H__

/// \file
///
/// This file contains common implementation of sinks.
/// Sink represent interface of buffer for writing data.
///
/// Usually sink has following methods:
///
/// - void reserve(size_t size)
/// - void append(const char *source, size_t sourceSize)
/// - size_t size() const
///
/// With this interface std::string is sink.
/// For other data structures we need to implement sink interface explicitly.

namespace strings
{

    /// Sink interface implementation for static char buffer
    class static_array_sink
    {
    public:
        typedef char char_t;

        static_array_sink(char_t *buffer, size_t bufferSize)
            : _buffer(buffer)
            , _bufferSize(bufferSize)
            , _currentOffset(0)
        {}

        template <size_t N>
        static_array_sink(char_t buffer[N])
            : _buffer(buffer)
            , _bufferSize(N)
            , _currentOffset(0)
        {}

        void reserve(size_t size)
        {
            if (size >= _bufferSize)
            {
                throw std::range_error("out of range: size");
            }
        }

        void append(const char_t *source, size_t sourceSize)
        {
            size_t copiedCharacters = string_copy(
                _buffer + _currentOffset,
                _bufferSize - _currentOffset,
                source,
                sourceSize);
            _currentOffset += copiedCharacters;
        }

        size_t size() const { return _currentOffset; }
    private:
        char_t *_buffer;
        size_t _bufferSize;
        size_t _currentOffset;
    };
}

#endif