#pragma once

#include <memory>

#ifdef MIST_PLATFORM_WINDOWS

#else
    #error Only supports Windows
#endif

#ifdef MIST_DEBUG
    #define MIST_ENABLE_ASSERTS
    #if defined(MIST_PLATFORM_WINDOWS)
        #define MIST_DEBUGBREAK __debugbreak()
    #elif defined(MIST_PLATFORM_LINUX)
        #define MIST_DEBUGBREAK raise(SIGTRAP)
    #else
        #error "Platform does not support DEBUGBREAK"
    #endif
#else
    #define MIST_DEBUGBREAK
#endif

#ifdef MIST_ENABLE_ASSERTS
    #define MIST_ASSERT(x, ...)                                                                                        \
        {                                                                                                              \
            if (!(x)) {                                                                                                \
                MIST_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                      \
                MIST_DEBUGBREAK;                                                                                       \
            }                                                                                                          \
        }
    #define MIST_CORE_ASSERT(x, ...)                                                                                   \
        {                                                                                                              \
            if (!(x)) {                                                                                                \
                MIST_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                 \
                MIST_DEBUGBREAK;                                                                                       \
            }                                                                                                          \
        }
#else
    #define MIST_ASSERT(x, ...)
    #define MIST_CORE_ASSERT(x, ...)
#endif

#define MIST_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)

namespace Mist {

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace Mist