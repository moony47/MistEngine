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
    void OnUpdateStart() override;
    void OnUpdateEnd() override;
    void OnEvent(Event& e) override;

    // private:
    //     bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    //     bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    //     bool OnMouseMovedEvent(MouseMovedEvent& e);
    //     bool OnMouseScrolledEvent(MouseScrolledEvent& e);
    //
    //     bool OnWindowResizedEvent(WindowResizeEvent& e);
    //
    //     bool OnKeyPressedEvent(KeyPressedEvent& e);
    //     bool OnKeyReleasedEvent(KeyReleasedEvent& e);
    //     bool OnKeyTypedEvent(KeyTypedEvent& e);

private:
    float m_Time = 0.0f;
};

} // namespace Mist
