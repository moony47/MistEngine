#include "mistpch.h"
#include "EditorCamera.h"

#include "Mist/Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mist {

EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
    m_FOV(fov),
    m_AspectRatio(aspectRatio),
    m_NearClip(nearClip),
    m_FarClip(farClip) {
    m_Modified = true;
    UpdateView();
}

void EditorCamera::UpdateProjection() {
    m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
    m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    m_Modified = false;
}

void EditorCamera::UpdateView() {
    m_Position = CalculatePosition();
    m_View = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(GetOrientation()));
}

glm::vec2 EditorCamera::PanSpeed() const {
    float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return {xFactor, yFactor};
}

float EditorCamera::RotationSpeed() const {
    return 0.8f;
}

float EditorCamera::ZoomSpeed() const {
    float distance = std::max(m_Distance, 0.0f);
    float speed = std::min(distance * distance * 0.09f, 100.0f);
    return speed;
}

void EditorCamera::OnUpdate(DeltaTime deltatime, bool focussed) {
    if (!m_Using && !focussed) {
        m_Using = false;
        return;
    }

    if (m_Using) {
        const glm::vec2& mouse{Input::GetMouseX(), Input::GetMouseY()};
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;

        if (Input::IsMouseButtonPressed(MouseButtonCode::Left)) {
            if (Input::IsKeyPressed(KeyCode::LeftShift))
                MouseZoom(delta.y);
            else if (Input::IsKeyPressed(KeyCode::LeftControl))
                MousePan(delta);
            else
                MouseRotate(delta);
        } else if (Input::IsMouseButtonPressed(MouseButtonCode::Middle))
            MousePan(delta);
        else if (Input::IsMouseButtonPressed(MouseButtonCode::Right))
            MouseZoom(delta.y);
        else
            m_Using = false;
    }

    UpdateView();
}

void EditorCamera::OnEvent(Event& e) {
    if (e.Handled)
        return;

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(MIST_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    dispatcher.Dispatch<MouseButtonPressedEvent>(MIST_BIND_EVENT_FN(EditorCamera::OnMousePressed));
}

bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
    float delta = e.GetYOffset() * 0.2f;
    MouseZoom(delta);
    UpdateView();
    return false;
}

bool EditorCamera::OnMousePressed(MouseButtonPressedEvent& e) {
    switch (e.GetMouseButton()) {
        case MouseButtonCode::Middle:
        case MouseButtonCode::Left:
        case MouseButtonCode::Right:
            m_InitialMousePosition = {Input::GetMouseX(), Input::GetMouseY()};
            m_Using = true;
            return true;
    }
    return false;
}

void EditorCamera::MousePan(const glm::vec2& delta) {
    auto speed = PanSpeed();
    m_FocalPoint += -GetRightDirection() * delta.x * speed.x * m_Distance;
    m_FocalPoint += GetUpDirection() * delta.y * speed.y * m_Distance;
}

void EditorCamera::MouseRotate(const glm::vec2& delta) {
    float sign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_Yaw += sign * delta.x * RotationSpeed();
    m_Pitch += delta.y * RotationSpeed();
}

void EditorCamera::MouseZoom(float delta) {
    m_Distance -= delta * ZoomSpeed();

    if (m_Distance < 1.0f) {
        m_FocalPoint += GetForwardDirection();
        m_Distance = 1.0f;
    }
}

glm::vec3 EditorCamera::GetUpDirection() const {
    return glm::rotate(GetOrientation(), {0.0f, 1.0f, 0.0f});
}

glm::vec3 EditorCamera::GetRightDirection() const {
    return glm::rotate(GetOrientation(), {1.0f, 0.0f, 0.0f});
}

glm::vec3 EditorCamera::GetForwardDirection() const {
    return glm::rotate(GetOrientation(), {0.0f, 0.0f, -1.0f});
}

glm::vec3 EditorCamera::CalculatePosition() const {
    return m_FocalPoint - GetForwardDirection() * m_Distance;
}

glm::quat EditorCamera::GetOrientation() const {
    return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}

} // namespace Mist