#pragma once

#include "Mist/Core/DeltaTime.h"
#include "Mist/Core/Layer.h"

namespace Mist {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& e) override;

    void OnImGuiRender(DeltaTime deltaTime) override;

    void Begin();
    void End();

    inline void SetPassEvents(bool block) {
        m_PassEvents = block;
    }

    void SetDarkThemeColours();

private:
    bool m_PassEvents = true;
    float m_Time = 0.0f;
};

} // namespace Mist
