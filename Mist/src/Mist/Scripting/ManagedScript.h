#pragma once

namespace Mist {

using managed_script = void*;
using on_create_fn = void (*)(managed_script);
using on_update_fn = void (*)(managed_script, float);
using on_destroy_fn = void (*)(managed_script);

class ManagedScript {
public:
    static on_create_fn s_OnCreateFunc;
    static on_update_fn s_OnUpdateFunc;
    static on_destroy_fn s_OnDestroyFunc;

public:
    ManagedScript(void* instance);
    ~ManagedScript();

    void OnCreate() const;
    void OnUpdate(float deltaTime) const;
    void OnDestroy() const;

private:
    void* m_ManagedScriptInstance;
};

} // namespace Mist
