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
    auto group = m_Registry.group<TransformComponent, SpriteComponent>();
    for (auto entity : group) {
        const auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
        Renderer2D::DrawQuad(transform.Position, transform.Rotation, transform.Scale, sprite.Colour,
                             sprite.TextureName);
    }
}

Entity& Scene::CreateEntity(const std::string& name) {
    Entity entity = Entity(this, m_Registry.create());
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
    return entity;
}

} // namespace Mist