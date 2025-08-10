#pragma once

#include "Core.h"
#include "Events/EventSystem.h"

namespace Mist {

class MIST_API Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {
    }
    virtual void OnDetach() {
    }
    virtual void OnUpdateStart() {
    }
    virtual void OnUpdateEnd() {
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