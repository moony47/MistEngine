#pragma once

#include "Components.h"
#include "Scene.h"

#include <entt.hpp>

namespace Mist {

class Entity {
public:
    Entity() = default;
    Entity(Scene* scene, entt::entity entityID);
    Entity(const Entity&) = default;

    UUID GetID() {
        return GetComponent<IDComponent>().ID;
    }
    const std::string& GetName() {
        return GetComponent<IDComponent>().Name;
    }

    template <typename T>
    bool HasComponent() {
        return m_Scene->m_Registry.any_of<T>(m_EntityID);
    }

    template <typename T>
    T& GetComponent() {
        MIST_CORE_ASSERT(HasComponent<T>(), "[Entity::GetComponent] Entity does not have such component");
        T& component = m_Scene->m_Registry.get<T>(m_EntityID);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T>
    T* TryGetComponent() {
        return m_Scene->m_Registry.try_get<T>(m_EntityID);
    }

    inline TransformComponent& Transform() {
        return GetComponent<TransformComponent>();
    }

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        MIST_CORE_ASSERT(!HasComponent<T>(), "[Entity::AddComponent] Entity already has such component");

        return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    T& AddOrReplaceComponent(Args&&... args) {
        return m_Scene->m_Registry.emplace_or_replace<T>(m_EntityID, std::forward<Args>(args)...);
    }

    template <typename T>
    void RemoveComponent() {
        MIST_CORE_ASSERT(HasComponent<T>(), "[Entity::RemoveComponent] Entity does not have such component");

        m_Scene->m_Registry.remove<T>(m_EntityID);
    }

    operator bool() const {
        return m_EntityID != entt::null;
    }
    operator entt::entity() const {
        return m_EntityID;
    }
    operator uint32_t() const {
        return (uint32_t)m_EntityID;
    }

    bool operator==(const Entity& other) const {
        return other.m_EntityID == m_EntityID && other.m_Scene == m_Scene;
    }
    bool operator!=(const Entity& other) const {
        return other.m_EntityID != m_EntityID || other.m_Scene != m_Scene;
    }

private:
    entt::entity m_EntityID{entt::null};
    Scene* m_Scene = nullptr;
};

} // namespace Mist