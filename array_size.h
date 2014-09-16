#ifndef __ARRAY_SIZE_HEADER_H__
#define __ARRAY_SIZE_HEADER_H__

#include <cstdint>

#ifndef __UNDOCUMENTED__
namespace Private
{
    template <int N>
    struct ArraySizeStruct
    {
        int8_t size[N];
    };

    template <class T, int N>
    ArraySizeStruct<N> GetStaticArraySizeFn(T (&)[N]);
}//end namespace Private
#endif//__UNDOCUMENTED__

#define ArraySize(array) sizeof(::Private::GetStaticArraySizeFn(array).size)

#endif
