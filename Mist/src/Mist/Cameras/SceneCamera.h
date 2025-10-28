#pragma once

#include "Mist/Cameras/Camera.h"

namespace Mist {

class SceneCamera : public Camera {
public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float nearClip, float farClip);
    void SetViewportSize(uint32_t width, uint32_t height);

    inline float GetOrthographicSize() const {
        return m_OrthoSize;
    }
    inline void SetOrthographicSize(float size) {
        m_OrthoSize = size;
        UpdateProjection();
    }

private:
    void UpdateProjection();

private:
    float m_OrthoSize = 10.0f;
    float m_OrthoNear = -1.0f, m_OrthoFar = 1.0f;
    float m_AspectRatio = 1.0f;
};

} // namespace Mist