#pragma once

#include <chrono>

using namespace std::chrono;

namespace Mist {

class Timer {
public:
    Timer() {
        Reset();
    }

    inline void Reset() {
        m_Start = high_resolution_clock::now();
    }

    inline float ElapsedSeconds() const {
        return ElapsedMilliseconds() * 0.001f;
    }
    inline float ElapsedMilliseconds() const {
        return duration_cast<nanoseconds>(high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f;
    }

private:
    time_point<high_resolution_clock> m_Start;
};

} // namespace Mist