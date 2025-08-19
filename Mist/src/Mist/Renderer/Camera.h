#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Mist {

class OrthographicCamera {
public:
    OrthographicCamera(float x, float y, float z, float left, float right, float bottom, float top) :
        m_View(1.0f),
        m_VP(1.0f),
        m_Proj(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
        m_Position(x, y, z) {
    }
    OrthographicCamera(glm::vec3& pos, float left, float right, float bottom, float top) :
        m_View(1.0f),
        m_VP(1.0f),
        m_Proj(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
        m_Position(pos) {
    }

    inline const glm::vec3& GetPosition() const {
        return m_Position;
    }

    void SetPosition(glm::vec3& pos) {
        m_Position = pos;
        modified = true;
    }
    void SetPosition(float x, float y, float z) {
        m_Position = glm::vec3(x, y, z);
        modified = true;
    }

    inline float GetRotation() const {
        return m_Rotation;
    }
    void SetRotation(float theta) {
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

    glm::mat4 GetVP() {
        if (modified) {
            Update();
            modified = false;
        }
        return m_VP;
    }

private:
    void Update() {
        glm::mat4 transform =
            glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)), m_Position);

        m_View = glm::inverse(transform);
        m_VP = m_Proj * m_View;
    }

private:
    bool modified = true;

    glm::mat4 m_View;
    glm::mat4 m_Proj;
    glm::mat4 m_VP;

    glm::vec3 m_Position;
    float m_Rotation = 0.0f;
};

} // namespace Mist