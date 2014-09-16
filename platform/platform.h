#ifndef __PLATFORM_HEADER_H__
#define __PLATFORM_HEADER_H__

#if defined(_WIN32)
#define PLATFORM_WIN32
#elif defined(linux) || defined(__GNUC__) || defined(__clang__)
#define PLATFORM_LINUX
#endif

#endif