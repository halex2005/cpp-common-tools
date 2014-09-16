#ifndef __THREAD_FUNCTIONS_H__
#define __THREAD_FUNCTIONS_H__

#include <cstddef>

namespace platform
{
    typedef size_t thread_id_t;
    thread_id_t current_thread_id();
}

#endif
