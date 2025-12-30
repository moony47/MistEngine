#pragma once

#include "Mist/Cameras/EditorCamera.h"
#include "Mist/Core/DeltaTime.h"
#include "Mist/Events/Event.h"
#include "Mist/Renderer/Renderer2D.h"

#include <entt.hpp>

namespace Mist {

class Entity;

class Scene {
public:
    Scene();
    ~Scene();

    static Ref<Scene> Copy(Ref<Scene> other);

    Entity& CreateEntity(const std::string& name = "");
    void DestroyEntity(Entity entity);

    void OnUpdate(DeltaTime deltaTime);
    void OnRender(DeltaTime deltaTime);

    // void OnUpdateEditor(DeltaTime deltaTime, EditorCamera& camera);
    void OnRenderEditor(DeltaTime deltaTime, EditorCamera& camera);

    void OnEvent(Event& e);

    void OnViewportResize(uint32_t width, uint32_t height);

    entt::registry& Get() {
        return m_Registry;
    }

    void SetPrimaryCamera(entt::entity id);
    Entity GetPrimaryCamera() const;

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);

private:
    entt::registry m_Registry;

    Entity* m_PrimaryCameraEntity = nullptr;
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

    friend class Entity;
    friend class SceneSerialiser;
    friend class SceneHierarchyPanel;
};

} // namespace Mist