#include "mistpch.h"
#include "Scene.h"

#include "Mist/Renderer/Texture.h"

#include "Mist/Scene/Components.h"
#include "Mist/Scene/Entity.h"
#include "Mist/Scene/ScriptableEntity.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Mist {

Scene::Scene() {};

Scene::~Scene() {};

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
    const glm::mat4& mainCameraTransform = m_PrimaryCameraEntity->GetComponent<TransformComponent>().GetTransform();

    // Render the scene from primary camera's view
    Renderer2D::BeginView(mainCamera->GetProjection(), mainCameraTransform);

    auto group = m_Registry.group<SpriteComponent, TransformComponent>();
    for (auto entity : group) {
        auto [sprite, transform] = group.get<SpriteComponent, TransformComponent>(entity);
        Renderer2D::DrawQuad(transform.GetTransform(), sprite.Colour, sprite.TextureName);
    }

    Renderer2D::EndView();
}

void Scene::OnEvent(Event& e) {
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

    // Adjust viewport size of primary camera
    CameraComponent& camera = m_PrimaryCameraEntity->GetComponent<CameraComponent>();
    if (!camera.FixedAspectRatio)
        camera.Camera.SetViewportSize(width, height);
}

Entity& Scene::CreateEntity(const std::string& name) {
    Entity* entity = new Entity(this, m_Registry.create());
    entity->AddComponent<TransformComponent>();
    entity->AddComponent<TagComponent>(name.empty() ? "Entity" : name);
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

template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
    static_assert(false);
}

template <>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {};
template <>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {};
template <>
void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component) {};
template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {};
template <>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {};

} // namespace Mist