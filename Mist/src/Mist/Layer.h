#pragma once

#include "Core.h"
#include "Mist/Events/EventSystem.h"
#include "Mist/Core/DeltaTime.h"

namespace Mist {

class MIST_API Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {
    }
    virtual void OnDetach() {
    }

    virtual void OnUpdateStart(DeltaTime deltaTime) {
    }
    virtual void OnUpdateEnd(DeltaTime deltaTime) {
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