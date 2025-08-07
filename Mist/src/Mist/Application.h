#pragma once
#include "Mist/Core.h"

#include "Mist/Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Window.h"

namespace Mist {

class MIST_API Application {
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};

Application* CreateApplication();

} // namespace Mist