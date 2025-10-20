#pragma once

namespace Mist {

class DeltaTime {
public:
    DeltaTime(float time = 0.0f) :
        m_Time(time) {
    }

    inline float GetSeconds() const {
        return m_Time;
    }
    inline float GetMilliseconds() const {
        return m_Time * 1000.0f;
    }
    inline float GetFPS() const {
        return 1.0f / m_Time;
    }

    inline operator float() const {
        return m_Time;
    }

private:
    float m_Time;
};

} // namespace Mist