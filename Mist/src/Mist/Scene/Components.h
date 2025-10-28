#pragma once
#include <glm/glm.hpp>
#include <string>

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
    glm::mat4 Transform = glm::mat4(1.0f);

    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& position = glm::vec3(0.0f),
                       const float rotation = 0.0f,
                       const glm::vec3& scale = glm::vec3(1.0f)) :
        Transform(1.0f) {
        Transform = glm::translate(Transform, position);
        Transform = glm::rotate(Transform, rotation, {0, 0, 1});
        Transform = glm::scale(Transform, scale);
    }
    TransformComponent(const glm::mat4& transform) :
        Transform(transform) {};

    inline void Translate(glm::vec3 position) {
        Transform = glm::translate(Transform, position);
    }
    inline void Rotate(float rotation) {
        Transform = glm::rotate(Transform, rotation, {0, 0, 1});
    }
    inline void Scale(glm::vec3 scale) {
        Transform = glm::scale(Transform, scale);
    }

    inline void SetPosition(glm::vec3& position) {
        Transform[3] = glm::vec4(position, 1);
    }
    // inline void SetRotation(float rotation) {
    //     m_Rotation = rotation;
    // }
    // inline void SetScale(glm::vec3& scale) {
    //     m_Scale = scale;
    // }

    inline const glm::vec3& GetPosition() const {
        return Transform[3];
    }
    // inline float GetRotation() const {
    //     return m_Rotation;
    // }
    // inline const glm::vec3& GetScale() const {
    //     return m_Scale;
    // }
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