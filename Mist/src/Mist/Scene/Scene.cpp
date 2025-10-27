#include "mistpch.h"
#include "Scene.h"

#include "Mist/Renderer/Texture.h"
#include "Mist/Scene/Components.h"
#include "Mist/Scene/Entity.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Mist {

Scene::Scene() {
    // entt::entity entity = m_Registry.create();
    // m_Registry.emplace<TransformComponent>(entity);

    // auto view = m_Registry.view<TransformComponent>();
    // for (auto entity : view) {
    //     TransformComponent& transform = view.get<TransformComponent>(entity);
    // }
}

Scene::~Scene() {
}

void Scene::OnUpdate(DeltaTime deltaTime) {
}

void Scene::OnRender(DeltaTime deltaTime) { 
    // Verify existence of and retrieve a primary camera
    Camera* mainCamera = nullptr;
    glm::mat4* mainCameraTransform = nullptr;
    if (m_PrimaryCameraEntity) {
        mainCamera = &m_PrimaryCameraEntity->TryGetComponent<CameraComponent>()->Camera;
        mainCameraTransform = &m_PrimaryCameraEntity->GetComponent<TransformComponent>().Tra;
    }

    if (mainCamera) {
        // Render the scene from primary camera's view
        Renderer2D::BeginView(mainCamera->GetProjection(), *mainCameraTransform);

        auto group = m_Registry.group<SpriteComponent, TransformComponent>();
        for (auto entity : group) {
            const auto& [sprite, transform] = group.get<SpriteComponent, TransformComponent>(entity);
            Renderer2D::DrawQuad(transform.Transform, sprite.Colour, sprite.TextureName);
        }

        Renderer2D::EndView();
    }
}

Entity& Scene::CreateEntity(const std::string& name) {
    Entity* entity = new Entity(this, m_Registry.create());
    entity->AddComponent<TransformComponent>();
    entity->AddComponent<TagComponent>(name.empty() ? "Entity" : name);
    return *entity;
}

} // namespace Mist