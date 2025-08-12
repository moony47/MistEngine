#pragma once

#ifdef MIST_PLATFORM_WINDOWS
    #if MIST_DYNAMIC_LINK
        #ifdef MIST_BUILD_DLL
            #define MIST_API __declspec(dllexport)
        #else
            #define MIST_API __declspec(dllimport)
        #endif
    #else
        #define MIST_API
    #endif
#else
    #error Only supports Windows
#endif

#ifdef MIST_DEBUG
    #define MIST_ENABLE_ASSERTS
#endif

#ifdef MIST_ENABLE_ASSERTS
    #define MIST_ASSERT(x, ...)                                                                                        \
        {                                                                                                              \
            if (!(x)) {                                                                                                \
                MIST_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                      \
                __debugbreak();                                                                                        \
            }                                                                                                          \
        }
    #define MIST_CORE_ASSERT(x, ...)                                                                                   \
        {                                                                                                              \
            if (!(x)) {                                                                                                \
                MIST_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                 \
                __debugbreak();                                                                                        \
            }                                                                                                          \
        }
#else
    #define MIST_ASSERT(x, ...)
    #define MIST_CORE_ASSERT(x, ...)
#endif

#define MIST_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)