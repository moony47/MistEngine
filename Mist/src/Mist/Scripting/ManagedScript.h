#pragma once

#include <entt.hpp>

namespace Mist {

class Entity;
class Scene;

using managed_script = void*;
using on_create_fn = void (*)(Entity*, managed_script);
using on_update_fn = void (*)(Entity*, managed_script, float);
using on_destroy_fn = void (*)(Entity*, managed_script);

class ManagedScript {
public:
    static on_create_fn s_OnCreateFunc;
    static on_update_fn s_OnUpdateFunc;
    static on_destroy_fn s_OnDestroyFunc;

public:
    ManagedScript(entt::entity entity, Scene* scene, managed_script instance);
    ~ManagedScript();

    void OnCreate() const;
    void OnUpdate(float deltaTime) const;
    void OnDestroy() const;

private:
    managed_script m_ManagedScriptInstance;
    Entity* m_Entity;
};

} // namespace Mist
