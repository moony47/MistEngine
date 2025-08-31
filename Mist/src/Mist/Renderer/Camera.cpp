#include "mistpch.h"

#include "Camera.h"

namespace Mist {

OrthographicCamera::OrthographicCamera(float x, float y, float z, float left, float right, float bottom, float top) :
    m_View(1.0f),
    m_VP(1.0f),
    m_Proj(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
    m_Position(x, y, z) {
}

OrthographicCamera::OrthographicCamera(const glm::vec3& pos, float left, float right, float bottom, float top) :
    m_View(1.0f),
    m_VP(1.0f),
    m_Proj(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
    m_Position(pos) {
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
    m_Proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    modified = true;
}

void OrthographicCamera::SetPosition(glm::vec3& pos) {
    m_Position = pos;
    modified = true;
}

void OrthographicCamera::SetPosition(float x, float y, float z) {
    m_Position = glm::vec3(x, y, z);
    modified = true;
}

void OrthographicCamera::SetRotation(float theta) {
    int t = (int)theta / 360;

    // Map angle to (-360, 360)
    if (t != 0)
        theta = theta - (t * 360);

    // Map angle to [0, 360)
    if (theta < 0)
        theta = 360 + theta;

    m_Rotation = theta;

    modified = true;
}

const glm::mat4& OrthographicCamera::GetVP() {
    if (modified) {
        UpdateVP();
        modified = false;
    }
    return m_VP;
}

void OrthographicCamera::UpdateVP() {
    PROFILE_FUNCTION();

    glm::mat4 transform =
        glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)), m_Position);

    m_View = glm::inverse(transform);
    m_VP = m_Proj * m_View;
}

} // namespace Mist
