#pragma once

#include "Mist/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Mist {

class Logger {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
        return s_CoreLogger;
    }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace Mist

// Logging macros
#define MIST_CORE_CRITICAL(...) ::Mist::Logger::GetCoreLogger()->critical(__VA_ARGS__)
#define MIST_CORE_ERROR(...) ::Mist::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define MIST_CORE_WARN(...) ::Mist::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define MIST_CORE_INFO(...) ::Mist::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define MIST_CORE_TRACE(...) ::Mist::Logger::GetCoreLogger()->trace(__VA_ARGS__)

#define MIST_CRITICAL(...) ::Mist::Logger::GetClientLogger()->critical(__VA_ARGS__)
#define MIST_ERROR(...) ::Mist::Logger::GetClientLogger()->error(__VA_ARGS__)
#define MIST_WARN(...) ::Mist::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define MIST_INFO(...) ::Mist::Logger::GetClientLogger()->info(__VA_ARGS__)
#define MIST_TRACE(...) ::Mist::Logger::GetClientLogger()->trace(__VA_ARGS__)