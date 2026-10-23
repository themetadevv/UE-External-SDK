#pragma once

#include <cstdint>

#if defined(_MSC_VER) // MSVC

#define DEBUG_BREAK          __debugbreak()	
#define PUSH_WARNING         __pragma(warning(push))
#define DISABLE_WARNING(x)   __pragma(warning(disable: x))
#define DISABLE_ALL_WARNING  __pragma(warning(push, 0))
#define RESET_WARNING        __pragma(warning(pop))

#elif defined(__GNUC__) // GCC

#define DEBUG_BREAK         __builtin_trap()
#define PUSH_WARNING        _Pragma("GCC diagnostic push")
#define DISABLE_WARNING(x)  _Pragma(#x)
#define DISABLE_ALL_WARNING \
				_Pragma("GCC diagnostic push") \
				_Pragma("GCC diagnostic ignored \"-Wall\"") \
				_Pragma("GCC diagnostic ignored \"-Wextra\"") \
				_Pragma("GCC diagnostic ignored \"-Wpedantic\"")
#define RESET_WARNING       _Pragma("GCC diagnostic pop")

#elif defined(__clang__) // CLANG

#define DEBUG_BREAK         __builtin_trap()
#define PUSH_WARNING        _Pragma("clang diagnostic push")
#define DISABLE_WARNING(x)  _Pragma(#x)
#define DISABLE_ALL_WARNING \
				_Pragma("clang diagnostic push") \
				_Pragma("clang diagnostic ignored \"-Weverything\"")
#define RESET_WARNING       _Pragma("clang diagnostic pop")

#else // UNKNOWN COMPILER
#error Unknown Compiler!
#define DEBUG_BREAK
#define PUSH_WARNING
#define DISABLE_WARNING(x)
#define DISABLE_ALL_WARNING
#define RESET_WARNING
#endif