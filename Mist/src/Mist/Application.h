#pragma once
#include "Mist/Core.h"

#include "Events/EventSystem.h"
#include "ImGui/ImGuiLayer.h"
#include "LayerStack.h"
#include "Window.h"

namespace Mist {

class MIST_API Application {
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    inline Window& GetWindow() {
        return *m_Window;
    }

    inline static Application& Get() {
        return *s_Instance;
    }

private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;

    LayerStack m_LayerStack;

private:
    static Application* s_Instance;
};

Application* CreateApplication();

} // namespace Mist