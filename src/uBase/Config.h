#pragma once

#if defined(WIN32)
# undef WIN32
# define WIN32 1
# if defined(_MSC_VER) && (_MSC_VER >= 1400)
#   define MSVC 1
# endif

#elif defined(ANDROID)
# undef ANDROID
# define ANDROID 1
# define __STDINT_LIMITS

#elif defined(__APPLE__)
# define APPLE 1
# include <TargetConditionals.h>
# if TARGET_OS_IPHONE
#   define IOS 1
# else
#   define OSX 1
# endif

#elif defined(__linux)
# define LINUX 1
# define __STDC_LIMIT_MACROS

#endif

#if !WIN32
# define UNIX 1
#endif

#if MSVC // Visual C++ specific
# define U_FUNCTION __FUNCTION__ //__FUNCSIG__
# define U_NORETURN __declspec(noreturn)
# ifdef _WINDLL
#  define U_DLLEXPORT __declspec(dllexport)
# else
#  define U_DLLEXPORT
# endif
#else
# define U_FUNCTION __PRETTY_FUNCTION__
# define U_NORETURN __attribute__((noreturn))
# define U_DLLEXPORT __attribute__((visibility("default")))
#endif
