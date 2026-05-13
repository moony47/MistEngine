#include "mistpch.h"
#include "DotNetRuntime.h"
#include "Mist/Core/Logger.h"
#include <filesystem>

#ifdef MIST_PLATFORM_WINDOWS
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

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
    std::vector<std::wstring> search_paths;

#ifdef MIST_PLATFORM_WINDOWS

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

                        MIST_INFO("Searching for hostfxr at: {0}", std::string(hostfxr_path.begin(), hostfxr_path.end()));

                        if (std::filesystem::exists(hostfxr_path)) {
                            hostfxr_lib = LoadLibraryW(hostfxr_path.c_str());
                            if (hostfxr_lib) {
                                loaded_path = hostfxr_path;
                                MIST_INFO("Successfully loaded hostfxr from: {0}", std::string(loaded_path.begin(), loaded_path.end()));
                                break;
                            }
                        }
                    }
                }
            }

            if (hostfxr_lib) break;
        }
        catch (const std::filesystem::filesystem_error& e) {
            MIST_WARN("Error searching directory {0}: {1}", std::string(fxr_base_path.begin(), fxr_base_path.end()), e.what());
        }
    }

    // Fallback: Try loading from PATH
    if (!hostfxr_lib) {
        MIST_WARN("Hostfxr 10.0 not found in standard paths, attempting to load from PATH");
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
    typedef int (*hostfxr_initialize_for_runtime_config_fn)(
        const wchar_t*,
        const void*,
        hostfxr_handle*);

    typedef int (*hostfxr_get_runtime_delegate_fn)(
        hostfxr_handle,
        int,
        void*);

    typedef int (*hostfxr_close_fn)(hostfxr_handle);

    auto get_hostfxr_initialize = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(
        GetProcAddress(hostfxr_lib, "hostfxr_initialize_for_runtime_config"));

    auto get_hostfxr_get_delegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(
        GetProcAddress(hostfxr_lib, "hostfxr_get_runtime_delegate"));

    auto get_hostfxr_close = reinterpret_cast<hostfxr_close_fn>(
        GetProcAddress(hostfxr_lib, "hostfxr_close"));

    if (!get_hostfxr_initialize || !get_hostfxr_get_delegate || !get_hostfxr_close) {
        MIST_ERROR("Failed to get hostfxr function pointers from hostfxr.dll");
        FreeLibrary(hostfxr_lib);
        return false;
    }

#else
    // Unix-like systems (Linux, macOS)
    std::vector<std::string> search_paths;

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

            if (hostfxr_lib) break;
        }
        catch (const std::filesystem::filesystem_error& e) {
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

    typedef int (*hostfxr_initialize_for_runtime_config_fn)(
        const char*,
        const void*,
        hostfxr_handle*);

    typedef int (*hostfxr_get_runtime_delegate_fn)(
        hostfxr_handle,
        int,
        void*);

    typedef int (*hostfxr_close_fn)(hostfxr_handle);

    auto get_hostfxr_initialize = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(
        dlsym(hostfxr_lib, "hostfxr_initialize_for_runtime_config"));

    auto get_hostfxr_get_delegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(
        dlsym(hostfxr_lib, "hostfxr_get_runtime_delegate"));

    auto get_hostfxr_close = reinterpret_cast<hostfxr_close_fn>(
        dlsym(hostfxr_lib, "hostfxr_close"));

    if (!get_hostfxr_initialize || !get_hostfxr_get_delegate || !get_hostfxr_close) {
        MIST_ERROR("Failed to get hostfxr function pointers from hostfxr library: {0}", dlerror());
        dlclose(hostfxr_lib);
        return false;
    }
#endif

    // Store the library handle and function pointers for later use
    m_HostfxrLibHandle = hostfxr_lib;
    m_HostfxrInitialize = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(get_hostfxr_initialize);
    m_HostfxrGetDelegate = get_hostfxr_get_delegate;
    m_HostfxrClose = get_hostfxr_close;

    MIST_INFO("Successfully loaded hostfxr library and obtained function pointers");
    return true;
}

bool DotNetRuntime::InitializeRuntime() {
    // TODO: Use hostfxr to initialize the .NET runtime
    // Steps:
    // 1. Find the runtime configuration (runtimeconfig.json)
    // 2. Call hostfxr_initialize_for_runtime_config
    // 3. Get the load_assembly_and_get_function_pointer delegate
    // 4. Store the entry point for later use

    MIST_WARN("InitializeRuntime not yet implemented - stub version");
    return true;
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
#else
        dlclose(m_HostfxrLibHandle);
#endif
        m_HostfxrLibHandle = nullptr;
    }

    // Clear function pointers
    m_HostfxrInitialize = nullptr;
    m_HostfxrGetDelegate = nullptr;
    m_HostfxrClose = nullptr;

    m_Initialized = false;
}

ManagedScript* DotNetRuntime::CreateInstance(const std::string& className) {
    if (!m_Initialized) {
        MIST_ERROR("Cannot create script instance: Runtime not initialized");
        return nullptr;
    }

    // TODO: Use reflection to create instances
    // Steps:
    // 1. Use System.Reflection to find the class by name
    // 2. Get the type's constructor
    // 3. Invoke it to create an instance
    // 4. Wrap the instance pointer in a ManagedScript wrapper
    // 5. Call OnCreate() on the new instance

    MIST_WARN("CreateInstance not yet implemented: {0}", className);
    return nullptr;
}

void DotNetRuntime::DestroyInstance(ManagedScript* instance) {
    if (!instance)
        return;

    // Call the script's cleanup method
    instance->OnDestroy();
    delete instance;
}

bool DotNetRuntime::IsInitialized() const {
    return m_Initialized;
}

} // namespace Mist
