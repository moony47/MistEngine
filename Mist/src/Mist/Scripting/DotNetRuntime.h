#pragma once

#include "IManagedRuntime.h"
#include <unordered_map>

namespace Mist {

// Forward declarations for .NET hosting API types
using hostfxr_handle = void*;

#ifdef MIST_PLATFORM_WINDOWS
    using hostfxr_lib_handle = void*;  // HMODULE
#else
    using hostfxr_lib_handle = void*;  // dlopen handle
#endif

// Function pointer types for hosting API
using hostfxr_initialize_for_runtime_config_fn = int(*)(const wchar_t*, const void*, hostfxr_handle*);
using hostfxr_initialize_for_runtime_config_unix_fn = int(*)(const char*, const void*, hostfxr_handle*);
using hostfxr_get_runtime_delegate_fn = int(*)(hostfxr_handle, int, void*);
using hostfxr_close_fn = int(*)(hostfxr_handle);

// .NET 10 runtime implementation
class DotNetRuntime : public IManagedRuntime {
public:
    DotNetRuntime() = default;
    virtual ~DotNetRuntime();

    bool Initialize(const std::string& assemblyPath) override;
    void Shutdown() override;

    // NEEDS fully qualified class name e.g. "GameScripts.PlayerController"
    ManagedScript* CreateInstance(const std::string& className) override;
    void DestroyInstance(ManagedScript* instance) override;

    bool IsInitialized() const override;

private:
    bool LoadHostfxr();
    bool InitializeRuntime();

private:
    bool m_Initialized = false;
    std::string m_AssemblyPath;

    // Library and context handles
    hostfxr_lib_handle m_HostfxrLibHandle = nullptr;    // Handle to hostfxr.dll/.so/.dylib
    hostfxr_handle m_HostfxrHandle = nullptr;            // Context handle from hostfxr API

    // Function pointers
    hostfxr_initialize_for_runtime_config_fn m_HostfxrInitialize = nullptr;
    hostfxr_get_runtime_delegate_fn m_HostfxrGetDelegate = nullptr;
    hostfxr_close_fn m_HostfxrClose = nullptr;
};

} // namespace Mist
