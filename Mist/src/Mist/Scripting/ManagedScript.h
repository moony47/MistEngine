#pragma once

namespace Mist {

class ManagedScript {
public:
    virtual ~ManagedScript() = default;

    virtual void OnCreate() {};
    virtual void OnUpdate(float deltaTime) {};
    virtual void OnDestroy() {};
};

} // namespace Mist
