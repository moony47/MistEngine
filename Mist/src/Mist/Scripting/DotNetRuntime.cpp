#include "mistpch.h"

#include "DotNetRuntime.h"

#include "Mist/Core/Logger.h"
#include <filesystem>

namespace Mist {

DotNetRuntime::~DotNetRuntime() {
    if (m_Initialized)
        Shutdown();
}

bool DotNetRuntime::Initialize(const std::string& assemblyPath) {
    MIST_INFO("Initializing .NET Runtime with assembly: {0}", assemblyPath);

    if (!std::filesystem::exists(assemblyPath)) {
        MIST_ERROR("Assembly not found: {0}", assemblyPath);
        return false;
    }

    m_AssemblyPath = assemblyPath;

    if (!LoadHostfxr()) {
        MIST_ERROR("Failed to load hostfxr");
        return false;
    }

    if (!InitializeRuntime()) {
        MIST_ERROR("Failed to initialize .NET runtime");
        return false;
    }

    m_Initialized = true;
    MIST_INFO(".NET Runtime initialized successfully");
    return true;
}

bool DotNetRuntime::LoadHostfxr() {
    // Load hostfxr library and get function pointers
    // hostfxr is located at: C:\Program Files\dotnet\host\fxr\<version>\hostfxr.dll

#ifdef MIST_PLATFORM_WINDOWS
    std::vector<std::wstring> search_paths;

    // 1. Try DOTNET_ROOT environment variable
    const char* dotnet_root_env = std::getenv("DOTNET_ROOT");
    if (dotnet_root_env) {
        std::wstring dotnet_root_wide(dotnet_root_env, dotnet_root_env + strlen(dotnet_root_env));
        search_paths.push_back(dotnet_root_wide + L"\\host\\fxr");
        MIST_INFO("Checking DOTNET_ROOT: {0}", dotnet_root_env);
    }

    // 2. Try common SDK installation paths
    search_paths.push_back(L"C:\\Program Files\\dotnet\\host\\fxr");
    search_paths.push_back(L"C:\\Program Files (x86)\\dotnet\\host\\fxr");

    // 3. Try user-specific installations
    const char* programfiles = std::getenv("ProgramFiles");
    if (programfiles) {
        std::wstring path_wide(programfiles, programfiles + strlen(programfiles));
        search_paths.push_back(path_wide + L"\\dotnet\\host\\fxr");
    }

    HMODULE hostfxr_lib = nullptr;
    std::wstring loaded_path;

    // Search for hostfxr.dll in versioned subdirectories
    for (const auto& fxr_base_path : search_paths) {
        try {
            // List subdirectories in the fxr folder to find version directories
            if (std::filesystem::exists(fxr_base_path)) {
                for (const auto& version_dir : std::filesystem::directory_iterator(fxr_base_path)) {
                    std::filesystem::path path = version_dir.path();
                    if (version_dir.is_directory() && path.filename().wstring().starts_with(L"10.0")) {
                        std::wstring hostfxr_path = path.wstring() + L"\\hostfxr.dll";

                        MIST_INFO("Searching for hostfxr at: {0}",
                                  std::string(hostfxr_path.begin(), hostfxr_path.end()));

                        if (std::filesystem::exists(hostfxr_path)) {
                            hostfxr_lib = LoadLibraryW(hostfxr_path.c_str());
                            if (hostfxr_lib) {
                                loaded_path = hostfxr_path;
                                MIST_INFO("Successfully loaded hostfxr from: {0}",
                                          std::string(loaded_path.begin(), loaded_path.end()));
                                break;
                            }
                        }
                    }
                }
            }

            if (hostfxr_lib)
                break;
        } catch (const std::filesystem::filesystem_error& e) {
            MIST_WARN("Error searching directory {0}: {1}", std::string(fxr_base_path.begin(), fxr_base_path.end()),
                      e.what());
        }
    }

    // Fallback: Try loading from PATH
    if (!hostfxr_lib) {
        MIST_WARN("Hostfxr not found in standard paths, attempting to load from PATH");
        hostfxr_lib = LoadLibraryW(L"hostfxr.dll");
    }

    if (!hostfxr_lib) {
        MIST_ERROR("Failed to load 10.0 hostfxr.dll. Ensure .NET 10 SDK or Runtime is installed.");
        MIST_ERROR("  - Expected location: C:\\Program Files\\dotnet\\host\\fxr\\<version>\\hostfxr.dll");
        MIST_ERROR("  - Download from: https://dotnet.microsoft.com/en-us/download/dotnet/10.0");
        MIST_ERROR("  - Or set DOTNET_ROOT environment variable to your .NET installation directory");
        return false;
    }

    // Get function pointers
    typedef int (*hostfxr_initialize_for_runtime_config_fn)(const wchar_t*, const void*, hostfxr_handle*);
    typedef int (*hostfxr_get_runtime_delegate_fn)(hostfxr_handle, int, void*);
    typedef int (*hostfxr_close_fn)(hostfxr_handle);

    auto get_hostfxr_initialize = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(
        GetProcAddress(hostfxr_lib, "hostfxr_initialize_for_runtime_config"));

    auto get_hostfxr_get_delegate =
        reinterpret_cast<hostfxr_get_runtime_delegate_fn>(GetProcAddress(hostfxr_lib, "hostfxr_get_runtime_delegate"));

    auto get_hostfxr_close = reinterpret_cast<hostfxr_close_fn>(GetProcAddress(hostfxr_lib, "hostfxr_close"));

    if (!get_hostfxr_initialize || !get_hostfxr_get_delegate || !get_hostfxr_close) {
        MIST_ERROR("Failed to get hostfxr function pointers from hostfxr.dll");
        FreeLibrary(hostfxr_lib);
        return false;
    }

#else
    // Unix-like systems (Linux, macOS)
    std::vector<std::wstring> search_paths;

    // 1. Try DOTNET_ROOT environment variable
    const char* dotnet_root_env = std::getenv("DOTNET_ROOT");
    if (dotnet_root_env) {
        search_paths.push_back(std::string(dotnet_root_env) + "/host/fxr");
        MIST_INFO("Checking DOTNET_ROOT: {0}", dotnet_root_env);
    }

    // 2. Try common installation paths
    #ifdef MIST_PLATFORM_LINUX
    search_paths.push_back("/usr/local/dotnet/host/fxr");
    search_paths.push_back("/opt/dotnet/host/fxr");
    const char* home = std::getenv("HOME");
    if (home) {
        search_paths.push_back(std::string(home) + "/.dotnet/host/fxr");
    }
    #elif defined(MIST_PLATFORM_MACOS)
    search_paths.push_back("/usr/local/opt/dotnet/host/fxr");
    const char* home = std::getenv("HOME");
    if (home) {
        search_paths.push_back(std::string(home) + "/.dotnet/host/fxr");
    }
    #endif

    #ifdef MIST_PLATFORM_LINUX
    const char* hostfxr_name = "libhostfxr.so";
    #elif defined(MIST_PLATFORM_MACOS)
    const char* hostfxr_name = "libhostfxr.dylib";
    #endif

    void* hostfxr_lib = nullptr;
    std::string loaded_path;

    // Search for hostfxr in versioned subdirectories
    for (const auto& fxr_base_path : search_paths) {
        try {
            if (std::filesystem::exists(fxr_base_path)) {
                for (const auto& version_dir : std::filesystem::directory_iterator(fxr_base_path)) {
                    std::filesystem::path path = version_dir.path();
                    if (version_dir.is_directory() && path.filename().string().starts_with(L"10.0")) {
                        std::string hostfxr_path = path.string() + "/" + hostfxr_name;

                        MIST_INFO("Searching for hostfxr at: {0}", hostfxr_path);

                        if (std::filesystem::exists(hostfxr_path)) {
                            hostfxr_lib = dlopen(hostfxr_path.c_str(), RTLD_LAZY);
                            if (hostfxr_lib) {
                                loaded_path = hostfxr_path;
                                MIST_INFO("Successfully loaded hostfxr from: {0}", loaded_path);
                                break;
                            }
                        }
                    }
                }
            }

            if (hostfxr_lib)
                break;
        } catch (const std::filesystem::filesystem_error& e) {
            MIST_WARN("Error searching directory {0}: {1}", fxr_base_path, e.what());
        }
    }

    // Fallback: Try loading from system library path
    if (!hostfxr_lib) {
        MIST_WARN("Hostfxr 10.0 not found in standard paths, attempting to load from system library path");
        hostfxr_lib = dlopen(hostfxr_name, RTLD_LAZY);
    }

    if (!hostfxr_lib) {
        MIST_ERROR("Failed to load hostfxr 10.0 library. Ensure .NET 10 SDK or Runtime is installed.");
        MIST_ERROR("  - Expected location: /usr/local/dotnet/host/fxr/<version>/libhostfxr.so");
        MIST_ERROR("  - Download from: https://dotnet.microsoft.com/en-us/download/dotnet/10.0");
        MIST_ERROR("  - Or set DOTNET_ROOT environment variable to your .NET installation directory");
        MIST_ERROR("  - Error: {0}", dlerror());
        return false;
    }

    typedef int (*hostfxr_initialize_for_runtime_config_fn)(const char*, const void*, hostfxr_handle*);

    typedef int (*hostfxr_get_runtime_delegate_fn)(hostfxr_handle, int, void*);

    typedef int (*hostfxr_close_fn)(hostfxr_handle);

    auto get_hostfxr_initialize = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(
        dlsym(hostfxr_lib, "hostfxr_initialize_for_runtime_config"));

    auto get_hostfxr_get_delegate =
        reinterpret_cast<hostfxr_get_runtime_delegate_fn>(dlsym(hostfxr_lib, "hostfxr_get_runtime_delegate"));

    auto get_hostfxr_close = reinterpret_cast<hostfxr_close_fn>(dlsym(hostfxr_lib, "hostfxr_close"));

    if (!get_hostfxr_initialize || !get_hostfxr_get_delegate || !get_hostfxr_close) {
        MIST_ERROR("Failed to get hostfxr function pointers from hostfxr library: {0}", dlerror());
        dlclose(hostfxr_lib);
        return false;
    }
#endif

    // Store the library handle and function pointers for later use
    m_HostfxrLibHandle = hostfxr_lib;
    m_HostfxrInitialize = get_hostfxr_initialize;
    m_HostfxrGetDelegate = get_hostfxr_get_delegate;
    m_HostfxrClose = get_hostfxr_close;

    MIST_INFO("Successfully loaded hostfxr library and obtained function pointers");
    return true;
}

bool DotNetRuntime::InitializeRuntime() {
    // Step 1: Find the runtime configuration (runtimeconfig.json)
    // hostfxr_initialize_for_runtime_config requires a .runtimeconfig.json file.
    // This file must be generated by the ScriptEngine.csproj with GenerateRuntimeConfigurationFiles=true
    std::filesystem::path assembly_path(m_AssemblyPath);
    std::string assembly_name = assembly_path.stem().string(); // e.g., "ScriptEngine" from "ScriptEngine.dll"
    std::filesystem::path assembly_dir = assembly_path.parent_path();

    std::filesystem::path runtime_config_path = assembly_dir / (assembly_name + ".runtimeconfig.json");

    if (!std::filesystem::exists(runtime_config_path)) {
        MIST_ERROR("Runtime configuration not found: {0}", runtime_config_path.string());
        MIST_ERROR("  - Expected: {0}.runtimeconfig.json in the same directory as the assembly", assembly_name);
        MIST_ERROR("  - This file MUST be generated during 'dotnet build' of the ScriptEngine C# project");
        MIST_ERROR("  - Ensure ScriptEngine.csproj has: "
                   "<GenerateRuntimeConfigurationFiles>true</GenerateRuntimeConfigurationFiles>");
        MIST_ERROR("  - Then rebuild: cd ScriptEngine && dotnet build -c Debug (or Release)");
        return false;
    }

    MIST_INFO("Found runtime configuration: {0}", runtime_config_path.string());

    // Step 2: Call hostfxr_initialize_for_runtime_config
    // This function initializes the runtime context for the given configuration.
    int result = 0;
    hostfxr_handle runtime_handle = nullptr;

#ifdef MIST_PLATFORM_WINDOWS
    std::wstring config_path_wide(runtime_config_path.wstring());
    result = m_HostfxrInitialize(config_path_wide.c_str(), nullptr, &runtime_handle);
#else
    std::string config_path_str = runtime_config_path.string();
    result = m_HostfxrInitialize(config_path_str.c_str(), nullptr, &runtime_handle);
#endif

    if (result != 0 || !runtime_handle) {
        MIST_ERROR("Failed to initialize .NET runtime with hostfxr (error code: {0})", result);
        MIST_ERROR("  - Config file: {0}", runtime_config_path.string());
        MIST_ERROR("  - Make sure the .NET 10 runtime is properly installed");
        MIST_ERROR("  - Ensure the assembly was built with 'dotnet build -c Release' or 'dotnet build -c Debug'");
        return false;
    }

    m_HostfxrHandle = runtime_handle;
    MIST_INFO("Successfully initialized .NET runtime context");

    // Step 3: Get the load_assembly_and_get_function_pointer delegate
    // This delegate is used to load assemblies and call C# methods from C++
    // Component ID: 5 = load_assembly_and_get_function_pointer
    typedef int (*load_assembly_fn)(const wchar_t*, // Assembly path
                                    const wchar_t*, // Type name (namespace.class)
                                    const wchar_t*, // Method name
                                    const wchar_t*, // Delegate type name (unused for our purposes)
                                    void*,          // Reserved
                                    void**);        // Output: pointer to function

    load_assembly_fn load_assembly_func = nullptr;
    result = m_HostfxrGetDelegate(runtime_handle, 5, &load_assembly_func);

    if (result != 0 || !load_assembly_func) {
        MIST_ERROR("Failed to get load_assembly_and_get_function_pointer delegate (error code: {0})", result);
        m_HostfxrClose(runtime_handle);
        m_HostfxrHandle = nullptr;
        return false;
    }

    MIST_INFO("Successfully obtained load_assembly_and_get_function_pointer delegate");
    m_LoadAssemblyFunc = load_assembly_func;

    // Step 4: Store handles for later use in CreateInstance
    // The runtime handle and function pointers are already stored in member variables
    // during LoadHostfxr() and here, ready to be used by CreateInstance()
    int size_needed =
        MultiByteToWideChar(CP_UTF8, 0, &assembly_path.string()[0], (int)assembly_path.string().size(), NULL, 0);
    std::wstring wide_assembly_path(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &assembly_path.string()[0], (int)assembly_path.string().size(),
                        &wide_assembly_path[0], size_needed);

    create_script_fn create_script = nullptr;
    on_create_fn on_create = nullptr;
    on_update_fn on_update = nullptr;
    on_destroy_fn on_destroy = nullptr;

    result = m_LoadAssemblyFunc(
        wide_assembly_path.c_str(),
        L"Mist.Scripting.ManagedScript, ScriptEngine",
        L"CreateScript", (const wchar_t*)(char*)-1, nullptr, (void**)&create_script);
    if (result != 0)
        MIST_ERROR("Failed to get ManagedScript.CreateScript function (error code: {0})", result);

    result = m_LoadAssemblyFunc(
        wide_assembly_path.c_str(),
        L"Mist.Scripting.ManagedScript, ScriptEngine",
        L"OnCreateNative", (const wchar_t*)(char*)-1, nullptr, (void**)&on_create);
    if (result != 0)
        MIST_ERROR("Failed to get ManagedScript.OnCreateNative function (error code: {0})", result);

    result = m_LoadAssemblyFunc(
        wide_assembly_path.c_str(),
        L"Mist.Scripting.ManagedScript, ScriptEngine",
        L"OnUpdateNative", (const wchar_t*)(char*)-1, nullptr, (void**)&on_update);
    if (result != 0)
        MIST_ERROR("Failed to get ManagedScript.OnUpdateNative function (error code: {0})", result);

    result = m_LoadAssemblyFunc(
        wide_assembly_path.c_str(),
        L"Mist.Scripting.ManagedScript, ScriptEngine",
        L"OnDestroyNative", (const wchar_t*)(char*)-1, nullptr, (void**)&on_destroy);
    if (result != 0)
        MIST_ERROR("Failed to get ManagedScript.OnDestroyNative function (error code: {0})", result);

    m_CreateScriptFunc = reinterpret_cast<create_script_fn>(create_script);
    ManagedScript::s_OnCreateFunc = reinterpret_cast<on_create_fn>(on_create);
    ManagedScript::s_OnUpdateFunc = reinterpret_cast<on_update_fn>(on_update);
    ManagedScript::s_OnDestroyFunc = reinterpret_cast<on_destroy_fn>(on_destroy);

    return true;
}

ManagedScript* DotNetRuntime::CreateInstance(const std::string& className) {
    if (!m_Initialized) {
        MIST_ERROR("Cannot create script instance: Runtime not initialized");
        return nullptr;
    }

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &className[0], (int)className.size(), NULL, 0);
    std::wstring wide_class_name(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &className[0], (int)className.size(), &wide_class_name[0], size_needed);

    ManagedScript* managed_script = new ManagedScript(m_CreateScriptFunc(wide_class_name.c_str()));

    MIST_INFO("{0} instance created", className);
    return managed_script;
}

void DotNetRuntime::Shutdown() {
    MIST_INFO("Shutting down .NET Runtime");

    // Close the hostfxr context if it exists
    if (m_HostfxrHandle && m_HostfxrClose) {
        m_HostfxrClose(m_HostfxrHandle);
        m_HostfxrHandle = nullptr;
    }

    // Unload the hostfxr library
    if (m_HostfxrLibHandle) {
#ifdef MIST_PLATFORM_WINDOWS
        FreeLibrary(static_cast<HMODULE>(m_HostfxrLibHandle));
        m_HostfxrLibHandle = nullptr;
#else
        dlclose(m_HostfxrLibHandle);
        m_HostfxrLibHandle = nullptr;
#endif
    }

    // Clear function pointers
    m_HostfxrLibHandle = nullptr;
    m_HostfxrHandle = nullptr;
    m_HostfxrInitialize = nullptr;
    m_HostfxrGetDelegate = nullptr;
    m_HostfxrClose = nullptr;

    m_LoadAssemblyFunc = nullptr;
    m_CreateScriptFunc = nullptr;

    ManagedScript::s_OnCreateFunc = nullptr;
    ManagedScript::s_OnUpdateFunc = nullptr;
    ManagedScript::s_OnDestroyFunc = nullptr;

    m_Initialized = false;
}

void DotNetRuntime::DestroyInstance(ManagedScript* instance) {
    if (instance)
        delete instance;
}

bool DotNetRuntime::IsInitialized() const {
    return m_Initialized;
}

} // namespace Mist
