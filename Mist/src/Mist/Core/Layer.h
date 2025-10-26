#pragma once

#include "Mist/Core/Core.h"
#include "Mist/Events/EventSystem.h"
#include "Mist/Core/DeltaTime.h"

namespace Mist {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {
    }
    virtual void OnDetach() {
    }

    virtual void OnUpdate(DeltaTime deltaTime) {
    }
    virtual void OnRender(DeltaTime deltaTime) {
    }
    virtual void OnImGuiRender(DeltaTime deltaTime) {
    }

    virtual void OnEvent(Event& e) {
    }

    inline const std::string& GetName() const {
        return m_DebugName;
    }

protected:
    std::string m_DebugName;
};

} // namespace Mist