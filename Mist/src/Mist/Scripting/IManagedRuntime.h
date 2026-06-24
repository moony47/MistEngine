#pragma once

#include "ManagedScript.h"
#include <string>
#include <memory>

namespace Mist {

class IManagedRuntime {
public:
    virtual ~IManagedRuntime() = default;

    virtual bool Initialize(const std::string& assemblyPath) = 0;
    virtual void Shutdown() = 0;

    virtual ManagedScript* CreateInstance(const std::string& className, entt::entity entity, Scene* scene) = 0;
    virtual void DestroyInstance(ManagedScript* instance) = 0;

    virtual bool IsInitialized() const = 0;
};

} // namespace Mist