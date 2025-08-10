#pragma once

#include "Mist/Layer.h"

// #include "Events/EventSystem.h"

namespace Mist {

class MIST_API ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& e) override;

    void OnImGuiRender() override;

    void Begin();
    void End();

private:
    float m_Time = 0.0f;
};

} // namespace Mist
