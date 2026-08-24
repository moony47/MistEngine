#include "mistpch.h"
#include "Scene.h"

#include "Mist/Scene/Components.h"
#include "Mist/Scene/Entity.h"
#include "Mist/Scene/ScriptableEntity.h"

#include <Mist/Cameras/Camera.h>
#include <Mist/Cameras/EditorCamera.h>
#include <Mist/Core/Core.h>
#include <Mist/Core/DeltaTime.h>
#include <Mist/Core/UUID.h>
#include <Mist/Events/Event.h>
#include <Mist/Renderer/Renderer2D.h>
#include <cstdint>
#include <entt.hpp>
#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>

namespace Mist {

template <typename... Component>
static void CopyComponentsAndUUIDs(entt::registry& dst,
                                   entt::registry& src,
                                   const std::unordered_map<UUID, entt::entity>& enttMap) {
    (
        [&]() {
            auto view = src.view<Component>();
            for (entt::entity srcEntity : view) {
                entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);
                Component& srcComponent = src.get<Component>(srcEntity);
                dst.emplace_or_replace<Component>(dstEntity, srcComponent);
            }
        }(),
        ...);
}

template <typename... Component>
static void CopyComponentsAndUUIDs(ComponentGroup<Component...>,
                                   entt::registry& dst,
                                   entt::registry& src,
                                   const std::unordered_map<UUID, entt::entity>& enttMap) {
    CopyComponentsAndUUIDs<Component...>(dst, src, enttMap);
}

template <typename... Component>
static void CopyComponentsIfExist(Entity dst, Entity src) {
    (
        [&]() {
            if (src.HasComponent<Component>())
                dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
        }(),
        ...);
}

template <typename... Component>
static void CopyComponentsIfExist(ComponentGroup<Component...>, Entity dst, Entity src) {
    CopyComponentsIfExist<Component...>(dst, src);
}

Scene::Scene(Ref<IManagedRuntime> runtime) :
    m_ManagedRuntime(runtime) {};

Scene::~Scene() {};

Ref<Scene> Scene::Copy(Ref<Scene> other) {
    Ref<Scene> newScene = CreateRef<Scene>(other->m_ManagedRuntime);

    newScene->m_ViewportWidth = other->m_ViewportWidth;
    newScene->m_ViewportHeight = other->m_ViewportHeight;
    if (other->m_PrimaryCameraEntity)
        newScene->m_PrimaryCameraEntity = new Entity{newScene.get(), (entt::entity)*other->m_PrimaryCameraEntity};

    auto& srcRegistry = other->m_Registry;
    auto& dstRegistry = newScene->m_Registry;
    std::unordered_map<UUID, entt::entity> enttMap;

    auto view = srcRegistry.view<IDComponent>();
    for (auto it = view.rbegin(); it != view.rend(); it++) {
        const IDComponent& idComp = srcRegistry.get<IDComponent>(*it);
        Entity newEntity = newScene->CreateEntity(idComp.ID, idComp.Name);
        enttMap[idComp.ID] = newEntity;
    }

    CopyComponentsAndUUIDs(AllComponents(), dstRegistry, srcRegistry, enttMap);

    return newScene;
}

void Scene::OnStart() {};

void Scene::OnUpdate(DeltaTime deltaTime) {
    m_Registry.view<NativeScriptComponent>().each(
        [this, deltaTime](entt::entity entity, NativeScriptComponent& script) {
            if (!script.Instance) {
                script.Instance = script.InstantiateScript();
                script.Instance->m_Entity = Entity{this, entity};
                script.Instance->OnCreate();
            }

            script.Instance->OnUpdate(deltaTime);
        });

    m_Registry.view<ManagedScriptComponent>().each(
        [this, deltaTime](entt::entity entity, ManagedScriptComponent& script) {
            // Create instance on first frame
            if (!script.Instance) {
                script.Instance = m_ManagedRuntime->CreateInstance(script.ScriptClassName, entity, this);
                if (!script.Instance) {
                    MIST_ERROR("Failed to create script: {0}", script.ScriptClassName);
                    return;
                }
            }

            // Call OnUpdate every frame
            if (script.Instance)
                script.Instance->OnUpdate(deltaTime);
        });
}

void Scene::RenderRenderableEntities() {
    auto group = m_Registry.group<RenderableComponent, TransformComponent>();

    group.sort<RenderableComponent>([](const RenderableComponent& first, const RenderableComponent& second) {
        return first.LayerZ > second.LayerZ;
    });

    for (auto entity : group) {
        auto [renderable, transform] = m_Registry.get<RenderableComponent, TransformComponent>(entity);
        Renderable type = renderable.GetType();
        switch (type) {
            case Renderable::Sprite: {
                SpriteComponent sprite = renderable.GetComponent<SpriteComponent>();
                Renderer2D::DrawQuad(
                    {(int)entity, transform.GetTransform(), sprite.Colour, sprite.TextureName, sprite.TilingFactor});
                auto& matrix = transform.GetTransform();
                Renderer2D::DrawRect({
                    (int)entity, matrix * glm::vec4{ 0.5f,  0.5f, 0.0f, 1.0f},
                    matrix * glm::vec4{-0.5f,  0.5f, 0.0f, 1.0f},
                       matrix * glm::vec4{-0.5f, -0.5f, 0.0f, 1.0f},
                    matrix * glm::vec4{ 0.5f, -0.5f, 0.0f, 1.0f},
                       glm::vec4(0.2f, 0.8f, 0.3f, 1.0f), 1.0f
                });
                break;
            }
            case Renderable::Circle: {
                CircleComponent circle = renderable.GetComponent<CircleComponent>();
                Renderer2D::DrawCircle(
                    {(int)entity, transform.GetTransform(), circle.Colour, circle.Thickness, circle.Fade});
                break;
            }
            case Renderable::Line: {
                LineComponent line = renderable.GetComponent<LineComponent>();
                Renderer2D::DrawLine({(int)entity, line.Point1, line.Point2, line.Colour, line.Thickness});
                break;
            }
        }
    }
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

    RenderRenderableEntities();

    Renderer2D::EndView();
}

void Scene::OnRenderEditor(DeltaTime deltaTime, EditorCamera& camera) {
    // Render the scene from primary camera's view
    Renderer2D::BeginView(camera);

    RenderRenderableEntities();

    Renderer2D::EndView();
}

void Scene::OnEvent(Event& e) {
    if (e.Handled)
        return;

    {
        auto view = m_Registry.view<NativeScriptComponent>();
        for(auto entity : view) {
            auto& script = view.get<NativeScriptComponent>(entity);
            if(script.Instance)
                script.Instance->OnEvent(e);
        }
    }
    {
        auto view = m_Registry.view<ManagedScriptComponent>();
        for(auto entity : view) {
            auto& script = view.get<ManagedScriptComponent>(entity);
            if(script.Instance)
                script.Instance->OnEvent(e);
        }
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

Entity Scene::CreateEntity(const UUID uuid, const std::string& name) {
    Entity entity(this, m_Registry.create());
    entity.AddComponent<IDComponent>(uuid, name);
    entity.AddComponent<TransformComponent>();
    return entity;
}

Entity Scene::DuplicateEntity(Entity entity) {
    Entity newEntity = CreateEntity();
    newEntity.GetComponent<IDComponent>().Name = entity.Name() + "_Copy";
    CopyComponentsIfExist(AllComponents(), newEntity, entity);
    return newEntity;
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
void Scene::OnComponentAdded<RenderableComponent>(Entity entity, RenderableComponent& component) {};
template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {};
template <>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {};
template <>
void Scene::OnComponentAdded<ManagedScriptComponent>(Entity entity, ManagedScriptComponent& component) {};

} // namespace Mist