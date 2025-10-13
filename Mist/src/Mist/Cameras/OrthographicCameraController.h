#pragma once

#include "Mist/Core/DeltaTime.h"

#include "Mist/Renderer/Camera.h"

#include "Mist/Events/EventSystem.h"

namespace Mist {

class OrthographicCameraController {
public:
    OrthographicCameraController(const glm::vec3& position, float ratio, bool rotation = false);
    OrthographicCameraController(float x, float y, float z, float ratio, bool rotation = false);

    void OnUpdate(DeltaTime deltaTime);
    void OnEvent(Event& e);

    inline OrthographicCamera& GetCamera() {
        return m_Camera;
    }

    inline float GetZoomLevel() {
        return m_ZoomLevel;
    }
    void SetZoomLevel(float level) {
        m_ZoomLevel = level;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    inline float GetLeft() {
        return -m_AspectRatio * m_ZoomLevel;
    }
    inline float GetRight() {
        return m_AspectRatio* m_ZoomLevel;
    }
    inline float GetBottom() {
        return -m_ZoomLevel;
    }
    inline float GetTop() {
        return m_ZoomLevel;
    }

private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    OrthographicCamera m_Camera;

    bool m_RotationEnabled;

    float m_CameraMoveSpeed = 5.0f;
    float m_CameraRotationSpeed = 60.0f;
};

} // namespace Mist