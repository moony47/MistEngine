#pragma once

#include "Mist/Cameras/Camera.h"

namespace Mist {

class SceneCamera : public Camera {
public:
    enum class CameraType {
        Perspective = 0,
        Orthographic = 1
    };

public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float nearClip, float farClip);
    void SetPerspective(float fov, float nearClip, float farClip);
    void SetViewportSize(uint32_t width, uint32_t height);

    inline void SetType(CameraType type) {
        m_CameraType = type;
        m_Modified = true;
    }
    inline CameraType GetType() {
        return m_CameraType;
    }

    inline void SetPerspectiveFOV(float fov) {
        m_PerspFOV = fov;
        m_Modified = true;
    }
    inline void SetPerspectiveNear(float nearClip) {
        m_PerspNear = nearClip;
        m_Modified = true;
    }
    inline void SetPerspectiveFar(float farClip) {
        m_PerspFar = farClip;
        m_Modified = true;
    }

    inline float GetPerspectiveFOV() const {
        return m_PerspFOV;
    }
    inline float GetPerspectiveNear() const {
        return m_PerspNear;
    }
    inline float GetPerspectiveFar() const {
        return m_PerspFar;
    }

    inline void SetOrthographicSize(float size) {
        m_OrthoSize = size;
        m_Modified = true;
    }
    inline void SetOrthographicNear(float nearClip) {
        m_OrthoNear = nearClip;
        m_Modified = true;
    }
    inline void SetOrthographicFar(float farClip) {
        m_OrthoFar = farClip;
        m_Modified = true;
    }

    inline float GetOrthographicSize() const {
        return m_OrthoSize;
    }
    inline float GetOrthographicNear() const {
        return m_OrthoNear;
    }
    inline float GetOrthographicFar() const {
        return m_OrthoFar;
    }

protected:
    void UpdateProjection() override;

private:

    CameraType m_CameraType = CameraType::Orthographic;

    float m_OrthoSize = 10.0f;
    float m_OrthoNear = -1.0f, m_OrthoFar = 1.0f;

    float m_PerspFOV = glm::radians(45.0f);
    float m_PerspNear = 0.01f, m_PerspFar = 1000.0f;

    float m_AspectRatio = 1.0f;
};

} // namespace Mist