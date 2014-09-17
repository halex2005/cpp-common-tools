#ifndef __ARRAY_SIZE_HEADER_H__
#define __ARRAY_SIZE_HEADER_H__

#include <cstdint>

#ifndef __DOXYGEN_RUNNING__

namespace Private
{
    template <int N>
    struct ArraySizeStruct
    {
        int8_t size[N];
    };

    template <class T, int N>
    ArraySizeStruct<N> GetStaticArraySizeFn(T (&)[N]);
}

#endif//__DOXYGEN_RUNNING__

/// \ingroup common-tools
/// \brief Compile-time compute length of static array.
///
/// Implementation is based on code in Andrei Alexandrescu book "C++ modern design".
///
/// Usage:
///
/// ~~~{.c}
/// char buffer[1024];
/// snprintf(buffer, ArraySize(buffer), "format %s", "string");
/// ~~~
#define ArraySize(array) sizeof(::Private::GetStaticArraySizeFn(array).size)

#endif
