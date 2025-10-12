#pragma once

#include "mistpch.h"

namespace Mist::Instrumentation {
struct ProfileResult {
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct InstrumentationSession {
    std::string Name;
};

class Instrumentor {
private:
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;

public:
    bool p_ProfilingEnabled;

    Instrumentor() :
        m_CurrentSession(nullptr),
        m_ProfileCount(0),
        p_ProfilingEnabled (true) {
    }

    void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
        if (!filepath.ends_with(".json"))
            m_OutputStream.open(filepath + ".json");
        else
            m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{name};
    }

    void EndSession() {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult& result) {
        if (!p_ProfilingEnabled || m_CurrentSession == nullptr)
            return;

        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{" << "\"cat\":\"function\"," << "\"dur\":" << (result.End - result.Start) << ','
                       << "\"name\":\"" << name << "\"," << "\"ph\":\"X\"," << "\"pid\":0,"
                       << "\"tid\":" << result.ThreadID << "," << "\"ts\":" << result.Start << "}";

        // if (m_OutputStream.)
        m_OutputStream.flush();
    }

    static Instrumentor& Get() {
        static Instrumentor instance;
        return instance;
    }

private:
    void WriteHeader() {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }
};

class InstrumentationTimer {
public:
    InstrumentationTimer(const char* name) :
        m_Name(name),
        m_Stopped(false) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer() {
        if (!m_Stopped)
            Stop();
    }

    void Stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end =
            std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({m_Name, start, end, threadID});

        m_Stopped = true;
    }

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};

#define PROFILE_ENABLED Mist::Instrumentation::Instrumentor::Get().p_ProfilingEnabled

#ifdef MIST_PROFILING
    #define PROFILE_SESSION_START(name, filepath)                                                                      \
        Mist::Instrumentation::Instrumentor::Get().BeginSession(name, filepath)
    #define PROFILE_SESSION_END() Mist::Instrumentation::Instrumentor::Get().EndSession()
    #define PROFILE_SCOPE(name) Mist::Instrumentation::InstrumentationTimer timer##__LINE__(name)
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
    #define PROFILE_SESSION_START(name, filepath)
    #define PROFILE_SESSION_END()
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
#endif

} // namespace Mist::Instrumentation