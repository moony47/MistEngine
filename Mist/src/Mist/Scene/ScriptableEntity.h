#pragma once

#include "Mist/Events/Event.h"
#include "Mist/Scene/Entity.h"

namespace Mist {

class ScriptableEntity {
public:
    virtual ~ScriptableEntity() = default;

    template <typename T>
    inline T& GetComponent() {
        return m_Entity.GetComponent<T>();
    }

    inline TransformComponent& Transform() {
        return m_Entity.GetComponent<TransformComponent>();
    }

protected:
    virtual void OnCreate() {};
    virtual void OnDestroy() {};
    virtual void OnUpdate(DeltaTime deltaTime) {};
    virtual void OnEvent(Event& e) {};

private:
    Entity m_Entity;
    friend class Scene;
};

} // namespace Mist