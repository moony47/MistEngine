#pragma once

#include "Mist/Scene/Entity.h"

namespace Mist::ScriptingCallbacks {

template <typename T>
static void GetComponent(Entity* entity, void** o_Component) {
    MIST_ASSERT(entity, "[ScriptingCallbacks::GetComponent<T>] - Entity argument is null");
    *o_Component = entity->TryGetComponent<T>();
}

static void __stdcall GetTransformComponent(Entity* entity, void** o_Component) {
    MIST_ASSERT(entity, "[ScriptingCallbacks::GetTransformComponent] - Entity argument is null");
    GetComponent<TransformComponent>(entity, o_Component);
}
static void __stdcall GetIDComponent(Entity* entity, void** o_Component) {
    MIST_ASSERT(entity, "[ScriptingCallbacks::GetIDComponent] - Entity argument is null");
    GetComponent<IDComponent>(entity, o_Component);
}
static void __stdcall GetCameraComponent(Entity* entity, void** o_Component) {
    MIST_ASSERT(entity, "[ScriptingCallbacks::GetCameraComponent] - Entity argument is null");
    GetComponent<CameraComponent>(entity, o_Component);
}

static void __stdcall SetTransformPosition(TransformComponent* transform, float x, float y, float z) {
    MIST_ASSERT(transform, "[ScriptingCallbacks::SetTransformPosition] - Transform argument is null");
    transform->SetPosition({x, y, z});
}
static void __stdcall GetTransformPosition(TransformComponent* transform, float* o_X, float* o_Y, float* o_Z) {
    MIST_ASSERT(transform, "[ScriptingCallbacks::SetTransformPosition] - Transform argument is null");
    MIST_ASSERT(o_X && o_Y && o_Z, "[ScriptingCallbacks::SetTransformPosition] - A position out-argument is null");

    glm::vec3 pos = transform->GetPosition();
    *o_X = pos.x;
    *o_Y = pos.y;
    *o_Z = pos.z;
}

} // namespace Mist::ScriptingCallbacks