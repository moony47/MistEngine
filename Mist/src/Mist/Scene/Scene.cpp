#include "mistpch.h"
#include "Scene.h"

#include "Mist/Renderer/Texture.h"

#include "Mist/Scene/Components.h"
#include "Mist/Scene/Entity.h"
#include "Mist/Scene/ScriptableEntity.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Mist {

template <typename... Component>
static void CopyComponent(entt::registry& dst,
                          entt::registry& src,
                          const std::unordered_map<UUID, entt::entity>& enttMap) {
    (
        [&]() {
            auto view = src.view<Component>();
            for (auto srcEntity : view) {
                entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

                auto& srcComponent = src.get<Component>(srcEntity);
                dst.emplace_or_replace<Component>(dstEntity, srcComponent);
            }
        }(),
        ...);
}

template <typename... Component>
static void CopyComponent(ComponentGroup<Component...>,
                          entt::registry& dst,
                          entt::registry& src,
                          const std::unordered_map<UUID, entt::entity>& enttMap) {
    CopyComponent<Component...>(dst, src, enttMap);
}

template <typename... Component>
static void CopyComponentIfExists(Entity dst, Entity src) {
    (
        [&]() {
            if (src.HasComponent<Component>())
                dst.AddComponent<Component>(src.GetComponent<Component>());
        }(),
        ...);
}

template <typename... Component>
static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src) {
    CopyComponentIfExists<Component...>(dst, src);
}

Scene::Scene() {};

Scene::~Scene() {};

Ref<Scene> Scene::Copy(Ref<Scene> other) {
    Ref<Scene> newScene = CreateRef<Scene>();

    newScene->m_ViewportWidth = other->m_ViewportWidth;
    newScene->m_ViewportHeight = other->m_ViewportHeight;
    newScene->m_PrimaryCameraEntity = other->m_PrimaryCameraEntity;

    auto& srcRegistry = other->m_Registry;
    auto& dstRegistry = newScene->m_Registry;
    std::unordered_map<UUID, entt::entity> enttMap;

    auto view = srcRegistry.view<IDComponent>();
    for (auto it = view.rbegin(); it != view.rend(); it++) {
        const IDComponent& idComp = srcRegistry.get<IDComponent>(*it);
        Entity newEntity = newScene->CreateEntity(idComp.ID, idComp.Tag);
        enttMap[idComp.ID] = newEntity;
    }

    CopyComponent(AllComponents{}, dstRegistry, srcRegistry, enttMap);

    return newScene;
}

void Scene::OnUpdate(DeltaTime deltaTime) {
    m_Registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& script) {
        if (!script.Instance) {
            script.Instance = script.InstantiateScript();
            script.Instance->m_Entity = Entity{this, entity};
            script.Instance->OnCreate();
        }

        script.Instance->OnUpdate(deltaTime);
    });
}

void Scene::OnRender(DeltaTime deltaTime) {
    if (!m_PrimaryCameraEntity)
        return;

    // Verify existence of and retrieve a primary camera
    Camera* mainCamera = &m_PrimaryCameraEntity->TryGetComponent<CameraComponent>()->Camera;
    if (!mainCamera)
        return;
    TransformComponent& transformComp = m_PrimaryCameraEntity->GetComponent<TransformComponent>();
    const glm::mat4& transformMatrix = transformComp.GetTransform();

    // Render the scene from primary camera's view
    Renderer2D::BeginView(mainCamera->GetProjection(), transformMatrix);

    auto group = m_Registry.group<SpriteComponent, TransformComponent>();
    group.sort<TransformComponent>(
        [pos = transformComp.GetPosition()](const TransformComponent& first, const TransformComponent& second) {
            return glm::distance2(first.GetPosition(), pos) > glm::distance2(second.GetPosition(), pos);
        });

    for (auto entity : group) {
        auto [sprite, transform] = group.get<SpriteComponent, TransformComponent>(entity);
        Renderer2D::DrawQuad({transform.GetTransform(), sprite.Colour, sprite.TextureName, sprite.TilingFactor});
    }

    Renderer2D::EndView();
}

// void Scene::OnUpdateEditor(DeltaTime deltaTime, EditorCamera& camera) {}

void Scene::OnRenderEditor(DeltaTime deltaTime, EditorCamera& camera) {
    // Render the scene from primary camera's view
    Renderer2D::BeginView(camera);

    auto group = m_Registry.group<SpriteComponent, TransformComponent>();
    group.sort<TransformComponent>(
        [pos = camera.GetPosition()](const TransformComponent& first, const TransformComponent& second) {
            return glm::distance2(first.GetPosition(), pos) > glm::distance2(second.GetPosition(), pos);
        });

    for (auto entity : group) {
        auto [sprite, transform] = group.get<SpriteComponent, TransformComponent>(entity);
        Renderer2D::DrawQuad(
            {(int)entity, transform.GetTransform(), sprite.Colour, sprite.TextureName, sprite.TilingFactor});
    }

    Renderer2D::EndView();
}

void Scene::OnEvent(Event& e) {
    if (e.Handled)
        return;

    auto view = m_Registry.view<NativeScriptComponent>();
    for (auto entity : view) {
        auto& script = view.get<NativeScriptComponent>(entity);
        if (script.Instance)
            script.Instance->OnEvent(e);
    }
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    if (!m_PrimaryCameraEntity)
        return;

    // Adjust viewport size of primary camera
    CameraComponent& camera = m_PrimaryCameraEntity->GetComponent<CameraComponent>();
    if (!camera.FixedAspectRatio)
        camera.Camera.SetViewportSize(width, height);
}

Entity& Scene::CreateEntity(const UUID uuid, const std::string& name) {
    Entity* entity = new Entity(this, m_Registry.create());
    entity->AddComponent<IDComponent>(uuid, name);
    entity->AddComponent<TransformComponent>();
    return *entity;
}

void Scene::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity);
}

void Scene::SetPrimaryCamera(entt::entity id) {
    if (m_PrimaryCameraEntity)
        delete m_PrimaryCameraEntity;

    m_PrimaryCameraEntity = new Entity(this, id);

    // Adjust viewport size of new primary camera
    CameraComponent& camera = m_PrimaryCameraEntity->GetComponent<CameraComponent>();
    if (!camera.FixedAspectRatio)
        camera.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
}

Entity Scene::GetPrimaryCamera() const {
    return *m_PrimaryCameraEntity;
}

template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
    static_assert(false);
}

template <>
void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {};
template <>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {};
template <>
void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component) {};
template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {};
template <>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {};

} // namespace Mist