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
    glm::mat4& mainCameraTransform = m_PrimaryCameraEntity->GetComponent<TransformComponent>().Transform;

    // Render the scene from primary camera's view
    Renderer2D::BeginView(mainCamera->GetProjection(), mainCameraTransform);

    auto group = m_Registry.group<SpriteComponent, TransformComponent>();
    for (auto entity : group) {
        auto [sprite, transform] = group.get<SpriteComponent, TransformComponent>(entity);
        Renderer2D::DrawQuad(transform.Transform, sprite.Colour, sprite.TextureName);
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

    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        CameraComponent& camera = view.get<CameraComponent>(entity);
        if (!camera.FixedAspectRatio)
            camera.Camera.SetViewportSize(width, height);
    }
}

Entity& Scene::CreateEntity(const std::string& name) {
    Entity* entity = new Entity(this, m_Registry.create());
    entity->AddComponent<TransformComponent>();
    entity->AddComponent<TagComponent>(name.empty() ? "Entity" : name);
    return *entity;
}

} // namespace Mist