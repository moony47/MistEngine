#pragma once

#include "Camera.h"
#include "Mist/Core/DeltaTime.h"
#include "Mist/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Mist {

class EditorCamera : public Camera {
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

    void OnUpdate(DeltaTime deltatime, bool focussed);
    void OnEvent(Event& e);

    inline float GetDistance() const {
        return m_Distance;
    }
    inline void SetDistance(float distance) {
        m_Distance = distance;
    }

    inline void SetViewportSize(float width, float height) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        m_Modified = true;
    }

    const glm::mat4& GetView() const {
        return m_View;
    }
    glm::mat4 GetViewProj() {
        UpdateProjection();
        return m_Projection * m_View;
    }

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;
    inline const glm::vec3& GetPosition() const {
        return m_Position;
    }
    glm::quat GetOrientation() const;

    inline float GetPitch() const {
        return m_Pitch;
    }
    inline float GetYaw() const {
        return m_Yaw;
    }

protected:
    void UpdateProjection() override;

private:
    void UpdateView();

    bool OnMouseScroll(MouseScrolledEvent& e);
    bool OnMousePressed(MouseButtonPressedEvent& e);

    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);

    glm::vec3 CalculatePosition() const;

    glm::vec2 PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

private:
    float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

    glm::mat4 m_View;
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};

    bool m_Using = false;
    glm::vec2 m_InitialMousePosition;

    float m_Distance = 10.0f, m_Pitch = 0.0f, m_Yaw = 0.0f;

    float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
};

} // namespace Mist