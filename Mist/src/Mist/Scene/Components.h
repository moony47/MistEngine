#pragma once
#include "Mist/Cameras/SceneCamera.h"
#include "Mist/Core/UUID.h"
#include "Mist/Scripting/ManagedScript.h"

#include <glm/glm.hpp>
#include <string>
#include <variant>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mist {

// Every entity must have an IDComponent for unique identification
struct IDComponent {
    UUID ID;
    std::string Name;

    IDComponent() = delete;
    IDComponent(const IDComponent&) = default;
    IDComponent(const UUID id, const std::string& name) :
        ID(id),
        Name(name) {};
};

// ------------------------ Renderables --------------------------

// Type mapping to allow following methods of using API:
//   1:  auto& sprite = renderable.GetComponent<SpriteComponent>();
//   2:  auto& sprite = renderable.GetComponent<Renderable::Sprite>();
//   3:  auto& sprite = renderable.GetComponent<Renderable::Sprite, SpriteComponent>();
// Similarly for SetComponent:
//   1:  auto& sprite = renderable.SetComponent<SpriteComponent>();
//   2:  auto& sprite = renderable.SetComponent<Renderable::Sprite>();
//   3:  auto& sprite = renderable.SetComponent<Renderable::Sprite, SpriteComponent>();
enum class Renderable : size_t {
    Sprite = 0,
    Circle = 1,
    Line = 2
};

template <Renderable T>
struct MapR2C;
template <typename T>
struct MapC2R;

struct SpriteComponent {
    std::string TextureName = "None";
    glm::vec4 Colour = {1.0f, 1.0f, 1.0f, 1.0f};
    float TilingFactor = 1.0f;

    SpriteComponent() = default;
    SpriteComponent(const SpriteComponent&) = default;
};

template <>
struct MapR2C<Renderable::Sprite> {
    using type = SpriteComponent;
};
template <>
struct MapC2R<SpriteComponent> {
    static constexpr Renderable type = Renderable::Sprite;
};

struct CircleComponent {
    glm::vec4 Colour{1.0f, 1.0f, 1.0f, 1.0f};
    float Thickness = 1.0f;
    float Fade = 0.005f;

    CircleComponent() = default;
    CircleComponent(const CircleComponent&) = default;
};

template <>
struct MapR2C<Renderable::Circle> {
    using type = CircleComponent;
};
template <>
struct MapC2R<CircleComponent> {
    static constexpr Renderable type = Renderable::Circle;
};

struct LineComponent {
    glm::vec3 Point1 = glm::vec3(0.0f);
    glm::vec3 Point2 = glm::vec3(0.0f);
    glm::vec4 Colour{1.0f, 1.0f, 1.0f, 1.0f};
    float Thickness = 1.0f;

    LineComponent() = default;
    LineComponent(const LineComponent&) = default;
};

template <>
struct MapR2C<Renderable::Line> {
    using type = LineComponent;
};
template <>
struct MapC2R<LineComponent> {
    static constexpr Renderable type = Renderable::Line;
};

struct RenderableComponent {
    float LayerZ = 0.0f;
    std::variant<SpriteComponent, CircleComponent, LineComponent> Component;

    inline Renderable GetType() const {
        return (Renderable)Component.index();
    }

    template <Renderable type, typename T = MapR2C<type>::type>
    T& GetComponent() {
        return std::get<T>(Component);
    }
    template <typename T>
    T& GetComponent() {
        return std::get<T>(Component);
    }

    template <Renderable type, typename T = MapR2C<type>::type>
    T& SetComponent() {
        return Component.emplace<(size_t)type>();
    }
    template <typename T, Renderable type = MapC2R<T>::type>
    T& SetComponent() {
        return Component.emplace<(size_t)type>();
    }
};
// ---------------------------------------------------------------

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

struct ManagedScriptComponent {
    ManagedScript* Instance = nullptr; // Pointer to C# managed script instance
    std::string ScriptClassName;       // Fully qualified C# class name (e.g., "GameScripts.PlayerController")
    UUID ScriptID;                     // Unique identifier for this script

    ManagedScriptComponent(std::string className) :
        ScriptClassName(className) {};
    ManagedScriptComponent(const ManagedScriptComponent&) = default;
};

template <typename... Component>
struct ComponentGroup {};

using AllComponents = ComponentGroup<TransformComponent,
                                     RenderableComponent,
                                     CameraComponent,
                                     NativeScriptComponent,
                                     ManagedScriptComponent>;

} // namespace Mist