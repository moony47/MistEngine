#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Mist {

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4& projection) :
        m_Projection(projection) {
    }
    virtual ~Camera() = default;

    const glm::mat4& GetProjection() {
        if (m_Modified)
            UpdateProjection();
        return m_Projection;
    }

protected:
    virtual void UpdateProjection() = 0;

protected:
    bool m_Modified = false;
    glm::mat4 m_Projection = glm::mat4(1.0f);
};

class OrthographicCamera {
public:
    OrthographicCamera(float x, float y, float z, float left, float right, float bottom, float top);
    OrthographicCamera(const glm::vec3& pos, float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);

    inline const glm::vec3& GetPosition() const {
        return m_Position;
    }
    void SetPosition(glm::vec3& pos);
    void SetPosition(float x, float y, float z);

    inline float GetRotation() const {
        return m_Rotation;
    }
    void SetRotation(float theta);

    const glm::mat4& GetVP();

private:
    void UpdateVP();

private:
    bool modified = true;

    glm::mat4 m_View;
    glm::mat4 m_Proj;
    glm::mat4 m_VP;

    glm::vec3 m_Position;
    float m_Rotation = 0.0f;
};

} // namespace Mist