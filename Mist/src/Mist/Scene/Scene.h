#pragma once

#include "Mist/Core/DeltaTime.h"
#include "Mist/Renderer/Renderer2D.h"

#include <entt.hpp>

namespace Mist {

class Entity;

class Scene {
public:
    Scene();
    ~Scene();

    Entity& CreateEntity(const std::string& name = "");

    void OnUpdate(DeltaTime deltaTime);
    void OnRender(DeltaTime deltaTime);

    entt::registry& Get() {
        return m_Registry;
    }

private:
    entt::registry m_Registry;

    friend class Entity;
};

} // namespace Mist