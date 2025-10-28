#include "mistpch.h"
#include "SceneCamera.h"

namespace Mist {

SceneCamera::SceneCamera() {
    UpdateProjection();
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
    m_OrthoSize = size;
    m_OrthoNear = nearClip;
    m_OrthoFar = farClip;
    UpdateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    UpdateProjection();
}

void SceneCamera::UpdateProjection() {
    float orthoLeft = -m_OrthoSize * m_AspectRatio * 0.5f;
    float orthoRight = m_OrthoSize * m_AspectRatio * 0.5f;
    float orthoBottom = -m_OrthoSize * 0.5f;
    float orthoTop = m_OrthoSize * 0.5f;
    m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthoNear, m_OrthoFar);
}

} // namespace Mist