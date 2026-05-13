#pragma once
#include "Mist/Core/Core.h"

#include "Mist/Core/LayerStack.h"
#include "Mist/Core/Window.h"
#include "Mist/Events/EventSystem.h"
#include "Mist/ImGui/ImGuiLayer.h"

#include "Mist/Core/DeltaTime.h"

namespace Mist {

struct ApplicationCommandLineArgs {
    int Count = 0;
    char** Args = nullptr;

    const char* operator[](int index) const {
        MIST_CORE_ASSERT(index < Count, "[ApplicationCommandLineArgs::operator[]] Index is out of range");
        return Args[index];
    }
};

class Application {
public:
    Application(const std::string& name = "Mist App", ApplicationCommandLineArgs args = {});
    virtual ~Application();

    void Run();
    void Close();

    void OnEvent(Event& e);

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> overlay);

    void PopLayer(Ref<Layer> layer);
    void PopOverlay(Ref<Layer> overlay);

    inline Ref<ImGuiLayer> GetImGuiLayer() {
        return m_ImGuiLayer;
    }

    inline Window& GetWindow() {
        return *m_Window;
    }
    inline ApplicationCommandLineArgs GetCommandLineArgs() const {
        return m_CommandLineArgs;
    }

    inline static Application& Get() {
        return *s_Instance;
    }

private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    ApplicationCommandLineArgs m_CommandLineArgs;

    LayerStack* m_LayerStack;
    LayerStack* m_NewLayerStack = nullptr;

    Scope<Window> m_Window;
    Ref<ImGuiLayer> m_ImGuiLayer;
    bool m_Running = true;
    bool m_Minimised = false;
    bool m_RunInBackground = true;

    float m_LastFrameTime = 0.0f;

private:
    static Application* s_Instance;
};

Application* CreateApplication(ApplicationCommandLineArgs args);

#define MIST_APP Application::Get()

} // namespace Mist