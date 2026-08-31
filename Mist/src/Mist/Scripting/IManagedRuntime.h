#pragma once

#include "ManagedScript.h"
#include "Mist/Core/Core.h"
#include <memory>
#include <string>

namespace Mist {

class IManagedRuntime {
private:
    static IManagedRuntime* s_ManagedRuntime;

public:
    static void InitManagedRuntime() {
#ifdef MIST_DEBUG
        s_ManagedRuntime->Initialize("..\\ScriptEngine\\bin\\Debug\\net10.0\\ScriptEngine.dll");
#elif MIST_RELEASE
        s_ManagedRuntime->Initialize("..\\ScriptEngine\\bin\\Release\\net10.0\\ScriptEngine.dll");
#elif MIST_DIST
        s_ManagedRuntime->Initialize("..\\ScriptEngine\\bin\\Dist\\net10.0\\ScriptEngine.dll");
#endif
    }
    static IManagedRuntime& GetManagedRuntime() {
        return *s_ManagedRuntime;
    }

public:
    virtual ~IManagedRuntime() = default;

    virtual bool Initialize(const std::string& assemblyPath) = 0;
    virtual void Shutdown() = 0;

    virtual ManagedScript* CreateInstance(const std::string& className, entt::entity entity, Scene* scene) = 0;
    virtual void DestroyInstance(ManagedScript* instance) = 0;

    virtual bool IsInitialized() const = 0;

    virtual std::vector<std::string> GetManagedScriptTypes(bool refresh) const = 0;
};

} // namespace Mist