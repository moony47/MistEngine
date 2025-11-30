#include "mistpch.h"

#include "OrthographicCameraController.h"

#include "Mist/Core/Input.h"
#include "Mist/Core/KeyCodes.h"

namespace Mist {

OrthographicCameraController::OrthographicCameraController(const glm::vec3& position, float ratio, bool rotation) :
    m_AspectRatio(ratio),
    m_Camera(position, -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
    m_RotationEnabled(rotation) {
}

OrthographicCameraController::OrthographicCameraController(float x, float y, float z, float ratio, bool rotation) :
    m_AspectRatio(ratio),
    m_Camera(x, y, z, -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
    m_RotationEnabled(rotation) {
}

void OrthographicCameraController::OnUpdate(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    glm::vec3 position = m_Camera.GetPosition();

    if (Input::IsKeyPressed(Key::D))
        position.x += deltaTime * m_CameraMoveSpeed * m_ZoomLevel;
    if (Input::IsKeyPressed(Key::A))
        position.x -= deltaTime * m_CameraMoveSpeed * m_ZoomLevel;
    if (Input::IsKeyPressed(Key::W))
        position.y += deltaTime * m_CameraMoveSpeed * m_ZoomLevel;
    if (Input::IsKeyPressed(Key::S))
        position.y -= deltaTime * m_CameraMoveSpeed * m_ZoomLevel;

    m_Camera.SetPosition(position);

    if (m_RotationEnabled) {
        float rotation = m_Camera.GetRotation();

        if (Input::IsKeyPressed(Key::Q))
            rotation += deltaTime * m_CameraRotationSpeed;
        if (Input::IsKeyPressed(Key::E))
            rotation -= deltaTime * m_CameraRotationSpeed;

        m_Camera.SetRotation(rotation);
    }
}

void OrthographicCameraController::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(MIST_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(MIST_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(float width, float height) {
    m_AspectRatio = width / height;
    UpdateProjection();
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    m_ZoomLevel = glm::clamp(m_ZoomLevel - e.GetYOffset() * 0.5f, 0.25f, 10.0f);
    UpdateProjection();
    return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
    OnResize((float)e.GetWidth(), (float)e.GetHeight());
    return false;
}

} // namespace Mist
