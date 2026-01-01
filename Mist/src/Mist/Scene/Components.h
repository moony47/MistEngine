#pragma once
#include "Mist/Cameras/SceneCamera.h"
#include "Mist/Core/UUID.h"

#include <glm/glm.hpp>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mist {

struct IDComponent {
    UUID ID;
    std::string Tag;

    IDComponent() = delete;
    IDComponent(const IDComponent&) = default;
    IDComponent(const UUID id, const std::string& tag) :
        ID(id),
        Tag(tag) {};
};

struct SpriteComponent {
    std::string TextureName;
    glm::vec4 Colour;
    float TilingFactor = 1.0f;

    SpriteComponent(const SpriteComponent&) = default;
    SpriteComponent(const std::string& textureName = "None",
                    const glm::vec4& colour = {1.0f, 1.0f, 1.0f, 1.0f},
                    float tilingFactor = 1.0f) :
        TextureName(textureName),
        Colour(colour),
        TilingFactor(tilingFactor) {};
    SpriteComponent(const glm::vec4& colour) :
        Colour(colour) {};
};

struct TransformComponent {

    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& position = glm::vec3(0.0f),
                       const glm::vec3& rotation = glm::vec3(0.0f),
                       const glm::vec3& scale = glm::vec3(1.0f)) :
        m_Position(position),
        m_Rotation(rotation),
        m_Scale(scale) {
        UpdateTransform();
    }

    inline void TranslateBy(const glm::vec3& position) {
        m_Position += position;
        m_Modified = true;
    }
    inline void RotateBy(const glm::vec3& rotation) {
        m_Rotation += rotation;
        m_Modified = true;
    }
    inline void ScaleBy(const glm::vec3& scale) {
        m_Scale += scale;
        m_Modified = true;
    }

    inline void SetPosition(glm::vec3 position) {
        m_Position = position;
        m_Modified = true;
    }
    inline void SetRotation(glm::vec3 rotation) {
        m_Rotation = rotation;
        m_Modified = true;
    }
    inline void SetScale(glm::vec3 scale) {
        m_Scale = scale;
        m_Modified = true;
    }

    inline const glm::vec3& GetPosition() const {
        return m_Position;
    }
    inline const glm::vec3& GetRotation() const {
        return m_Rotation;
    }
    inline const glm::vec3& GetScale() const {
        return m_Scale;
    }

    inline const glm::mat4& GetTransform() {
        if (m_Modified)
            UpdateTransform();
        return m_Transform;
    }

private:
    void UpdateTransform() {
        m_Transform = glm::translate(glm::mat4(1.0f), m_Position);
        m_Transform *= glm::toMat4(glm::quat(m_Rotation));
        m_Transform = glm::scale(m_Transform, m_Scale);
        m_Modified = false;
    }

private:
    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::vec3 m_Rotation = glm::vec3(0.0f);
    glm::vec3 m_Scale = glm::vec3(1.0f);

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

template <typename... Component>
struct ComponentGroup {};

using AllComponents = ComponentGroup<TransformComponent, SpriteComponent, CameraComponent, NativeScriptComponent>;

} // namespace Mist