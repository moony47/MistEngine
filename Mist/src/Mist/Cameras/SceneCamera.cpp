#include "mistpch.h"
#include "SceneCamera.h"

namespace Mist {

SceneCamera::SceneCamera() {
    UpdateProjection();
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
    m_CameraType = CameraType::Orthographic;
    m_OrthoSize = size;
    m_OrthoNear = nearClip;
    m_OrthoFar = farClip;
    m_Modified = true;
}

void SceneCamera::SetPerspective(float fov, float nearClip, float farClip) {
    m_CameraType = CameraType::Perspective;
    m_PerspFOV = fov;
    m_PerspNear = nearClip;
    m_PerspFar = farClip;
    m_Modified = true;
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    m_Modified = true;
}

void SceneCamera::UpdateProjection() {
    if (m_CameraType == CameraType::Orthographic) {
        float orthoLeft = -m_OrthoSize * m_AspectRatio * 0.5f;
        float orthoRight = m_OrthoSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthoSize * 0.5f;
        float orthoTop = m_OrthoSize * 0.5f;
        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthoNear, m_OrthoFar);
    } else {
        m_Projection = glm::perspective(m_PerspFOV, m_AspectRatio, m_PerspNear, m_PerspFar);
    }
    m_Modified = false;
}

} // namespace Mist