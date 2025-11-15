#pragma once
#include <glm/glm.hpp>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Mist/Cameras/SceneCamera.h"

namespace Mist {

struct TagComponent {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag) :
        Tag(tag) {};
};

struct SpriteComponent {
    std::string TextureName;
    glm::vec4 Colour;

    SpriteComponent(const SpriteComponent&) = default;
    SpriteComponent(const std::string& textureName = "WHITE", const glm::vec4& colour = {1.0f, 1.0f, 1.0f, 1.0f}) :
        TextureName(textureName),
        Colour(colour) {};
    SpriteComponent(const glm::vec4& colour) :
        Colour(colour) {};
};

struct TransformComponent {
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);

    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& position = glm::vec3(0.0f),
                       const glm::vec3& rotation = glm::vec3(0.0f),
                       const glm::vec3& scale = glm::vec3(1.0f)) :
        Position(position),
        Rotation(rotation),
        Scale(scale) {
        UpdateTransform();
    }

    inline void TranslateBy(const glm::vec3& position) {
        Position += position;
        m_Modified = true;
    }
    inline void RotateBy(const glm::vec3& rotation) {
        Rotation += rotation;
        m_Modified = true;
    }
    inline void ScaleBy(const glm::vec3& scale) {
        Scale += scale;
        m_Modified = true;
    }

    inline void SetPosition(glm::vec3 position) {
        Position = position;
        m_Modified = true;
    }
    inline void SetRotation(glm::vec3 rotation) {
        Rotation = rotation;
        m_Modified = true;
    }
    inline void SetScale(glm::vec3 scale) {
        Scale = scale;
        m_Modified = true;
    }

    inline const glm::mat4& GetTransform() {
        if (m_Modified)
            UpdateTransform();
        return m_Transform;
    }

private:
    void UpdateTransform() {
        m_Transform = glm::translate(glm::mat4(1.0f), Position);
        m_Transform = glm::toMat4(glm::quat(Rotation));
        //m_Transform = glm::rotate(m_Transform, Rotation.x, {1, 0, 0});
        //m_Transform = glm::rotate(m_Transform, Rotation.y, {0, 1, 0});
        //m_Transform = glm::rotate(m_Transform, Rotation.z, {0, 0, 1});
        m_Transform = glm::scale(m_Transform, Scale);
        m_Modified = false;
    }

private:
    glm::mat4 m_Transform;
    bool m_Modified;
};

struct CameraComponent {
    SceneCamera Camera;
    bool FixedAspectRatio = false;

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
};

class ScriptableEntity;

struct NativeScriptComponent {
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity* (*InstantiateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template <typename T>
        requires std::is_base_of<ScriptableEntity, T>::value
    void Bind() {
        InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
        DestroyScript = [](NativeScriptComponent* ncs) {
            delete ncs->Instance;
            ncs->Instance = nullptr;
        };
    }
};

} // namespace Mist