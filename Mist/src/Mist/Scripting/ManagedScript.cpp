#include "mistpch.h"
#include "ManagedScript.h"

#include "Mist/Scene/Entity.h"

namespace Mist {

on_create_fn ManagedScript::s_OnCreateFunc = nullptr;
on_update_fn ManagedScript::s_OnUpdateFunc = nullptr;
on_destroy_fn ManagedScript::s_OnDestroyFunc = nullptr;

ManagedScript::ManagedScript(entt::entity entity, Scene* scene, managed_script instance) :
    m_Entity(new Entity{scene, entity}),
    m_ManagedScriptInstance(instance){
    OnCreate();
}

ManagedScript::~ManagedScript() {
    OnDestroy();
    delete m_ManagedScriptInstance;
}

void ManagedScript::OnCreate() const {
    MIST_ASSERT(s_OnCreateFunc, "Dotnet managed script OnCreate function point is not initialised")

    s_OnCreateFunc(m_Entity, m_ManagedScriptInstance);
}
void ManagedScript::OnUpdate(float deltaTime) const {
    MIST_ASSERT(s_OnUpdateFunc, "Dotnet managed script OnUpdate function point is not initialised")

    s_OnUpdateFunc(m_Entity, m_ManagedScriptInstance, deltaTime);
}
void ManagedScript::OnDestroy() const {
    MIST_ASSERT(s_OnDestroyFunc, "Dotnet managed script OnDestroy function point is not initialised")

    s_OnDestroyFunc(m_Entity, m_ManagedScriptInstance);
}

} // namespace Mist