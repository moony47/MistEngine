#include "mistpch.h"
#include "ManagedScript.h"

namespace Mist {

on_create_fn ManagedScript::s_OnCreateFunc = nullptr;
on_update_fn ManagedScript::s_OnUpdateFunc = nullptr;
on_destroy_fn ManagedScript::s_OnDestroyFunc = nullptr;

ManagedScript::ManagedScript(void* instance) :
    m_ManagedScriptInstance(instance) {
    OnCreate();
}

ManagedScript::~ManagedScript() {
    OnDestroy();
    delete m_ManagedScriptInstance;
}

void ManagedScript::OnCreate() const {
    s_OnCreateFunc(m_ManagedScriptInstance);
}
void ManagedScript::OnUpdate(float deltaTime) const {
    s_OnUpdateFunc(m_ManagedScriptInstance, deltaTime);
}
void ManagedScript::OnDestroy() const {
    s_OnDestroyFunc(m_ManagedScriptInstance);
}

} // namespace Mist