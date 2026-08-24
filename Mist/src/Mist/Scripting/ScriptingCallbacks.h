#pragma once

#include "Mist/Scene/Entity.h"
#include "Mist/Core/Input.h"
#include <combaseapi.h>

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

static void __stdcall GetID(IDComponent* comp, uint64_t* o_ID) {
    MIST_ASSERT(comp, "[ScriptingCallbacks::GetID] - Component argument is null");
    *o_ID = comp->ID;
}
static void __stdcall GetName(IDComponent* comp, wchar_t** o_Name) {
    MIST_ASSERT(comp, "[ScriptingCallbacks::GetName] - Component argument is null");
    MIST_ASSERT(o_Name, "[ScriptingCallbacks::GetName] - Output name pointer is null");

    const std::string& name = comp->Name;

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), (int) name.size(), NULL, 0);

    wchar_t* wideName = static_cast<wchar_t*>(CoTaskMemAlloc((size_needed + 1) * sizeof(wchar_t)));
    MIST_ASSERT(wideName, "[ScriptingCallbacks::GetName] - Failed to allocate memory");

    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), (int) name.size(), wideName, size_needed);
    wideName[size_needed] = L'\0';

    *o_Name = wideName;
}

static void __stdcall SetTransformPosition(TransformComponent* transform, float x, float y, float z) {
    MIST_ASSERT(transform, "[ScriptingCallbacks::SetTransformPosition] - Transform argument is null");
    transform->SetPosition({x, y, z});
}
static void __stdcall GetTransformPosition(TransformComponent* transform, float* o_X, float* o_Y, float* o_Z) {
    MIST_ASSERT(transform, "[ScriptingCallbacks::SetTransformPosition] - Transform argument is null");
    MIST_ASSERT(o_X && o_Y && o_Z, "[ScriptingCallbacks::SetTransformPosition] - A position pointer is null");

    glm::vec3 pos = transform->GetPosition();
    *o_X = pos.x;
    *o_Y = pos.y;
    *o_Z = pos.z;
}

static void __stdcall IsKeyPressed(uint16_t key, bool* o_Pressed) {
    *o_Pressed = Input::IsKeyPressed(static_cast<Key>(key));
}
static void __stdcall IsMouseButtonPressed(uint16_t mouseButton, bool* o_Pressed) {
    *o_Pressed = Input::IsMouseButtonPressed(static_cast<MouseButton>(mouseButton));
}
static void __stdcall GetMousePosition(uint16_t key, float* o_X, float* o_Y) {
    auto pos = Input::GetMousePosition();
    *o_X = pos.first;
    *o_Y = pos.second;
}

} // namespace Mist::ScriptingCallbacks