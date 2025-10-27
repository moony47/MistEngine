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

    template <typename T>
    bool HasComponent() {
        return m_Scene->m_Registry.any_of<T>(m_EntityID);
    }

    template <typename T>
    T& GetComponent() {
        MIST_CORE_ASSERT(HasComponent<T>(), "[Entity::GetComponent] Entity does not have such component");

        return m_Scene->m_Registry.get<T>(m_EntityID);
    }

    template <typename T>
    T* TryGetComponent() {
        MIST_CORE_ASSERT(HasComponent<T>(), "[Entity::GetComponent] Entity does not have such component");

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

    template <typename T>
    void RemoveComponent() {
        MIST_CORE_ASSERT(HasComponent<T>(), "[Entity::RemoveComponent] Entity does not have such component");

        m_Scene->m_Registry.remove<T>(m_EntityID);
    }

    operator bool() const {
        return m_EntityID != entt::null;
    }

private:
    entt::entity m_EntityID{entt::null};
    Scene* m_Scene = nullptr;
};

} // namespace Mist