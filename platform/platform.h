#ifndef __PLATFORM_HEADER_H__
#define __PLATFORM_HEADER_H__

#if defined(_WIN32)
#define PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#elif defined(linux) || defined(__GNUC__) || defined(__clang__)
#define PLATFORM_LINUX
#endif

#ifdef __DOXYGEN_RUNNING__

/// \defgroup platform platform
/// \ingroup common-tools
///
/// This group contains implementations for common tools, based on specific platform support.
///
/// \addtogroup platform
/// @{

#ifndef PLATFORM_WIN32
/// \brief Defined when compiling under WIN32 platform.
#define PLATFORM_WIN32
#undef  PLATFORM_WIN32
#endif

#ifndef PLATFORM_LINUX
/// \brief Defined when compiling under LINUX platform.
#define PLATFORM_LINUX
#undef  PLATFORM_LINUX
#endif

/// \brief Contains platform functions and classes
namespace platform
{
}

/// @}

#endif

#endif