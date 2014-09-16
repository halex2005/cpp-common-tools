#include "thread_functions.h"
#include "platform.h"

// current_thread_id() implementation
// ==================================
#if defined(PLATFORM_WIN32)
namespace platform
{
    thread_id_t current_thread_id()
    {
        return thread_id_t(::GetCurrentThreadId());
    }
}
#elif defined(PLATFORM_LINUX)
#include <pthread.h>
namespace platform
{
    thread_id_t current_thread_id()
    {
        return pthread_self();
    }
}
#else
#error Platform not supported
#endif