#pragma once

#include "ManagedScript.h"
#include "IManagedRuntime.h"
#include <unordered_map>
#include <entt.hpp>

namespace Mist {

// Forward declarations for .NET hosting API types
using hostfxr_handle = void*;
using hostfxr_lib_handle = void*;

// Function pointer types for hosting API
using hostfxr_initialize_for_runtime_config_fn = int (*)(const wchar_t*, const void*, hostfxr_handle*);
using hostfxr_initialize_for_runtime_config_unix_fn = int (*)(const char*, const void*, hostfxr_handle*);
using hostfxr_get_runtime_delegate_fn = int (*)(hostfxr_handle, int, void*);
using hostfxr_close_fn = int (*)(hostfxr_handle);
using load_assembly_fn = int (*)(const wchar_t*, const wchar_t*, const wchar_t*, const wchar_t*, void*, void**);

// Function pointer types for ManagedScript .NET class
using create_script_fn = managed_script (*)(const wchar_t*);
using register_callback_fn = int (*)(const wchar_t*, void*);

class Scene;

// .NET 10 runtime implementation
class DotNetRuntime : public IManagedRuntime {
public:
    DotNetRuntime() = default;
    virtual ~DotNetRuntime();

    bool Initialize(const std::string& assemblyPath) override;
    void Shutdown() override;

    // NEEDS fully qualified class name e.g. "GameScripts.PlayerController"
    ManagedScript* CreateInstance(const std::string& className, entt::entity entity, Scene* scene) override;
    void DestroyInstance(ManagedScript* instance) override;

    bool IsInitialized() const override;

private:
    bool LoadHostfxr();
    bool InitializeRuntime();
    bool RegisterCallbacks();

private:
    bool m_Initialized = false;
    std::string m_AssemblyPath;

    // Library and context handles
    hostfxr_lib_handle m_HostfxrLibHandle = nullptr;
    hostfxr_handle m_HostfxrHandle = nullptr;

    // Function pointers
    hostfxr_initialize_for_runtime_config_fn m_HostfxrInitialize = nullptr;
    hostfxr_get_runtime_delegate_fn m_HostfxrGetDelegate = nullptr;
    hostfxr_close_fn m_HostfxrClose = nullptr;
    load_assembly_fn m_LoadAssemblyFunc = nullptr;
    create_script_fn m_CreateScriptFunc = nullptr;
    register_callback_fn m_RegisterCallback = nullptr;
};

} // namespace Mist
