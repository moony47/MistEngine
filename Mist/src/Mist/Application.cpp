#include "mistpch.h"

#include "Application.h"
#include "Input.h"
#include "Logger.h"

namespace Mist {

Application* Application::s_Instance = nullptr;

Application::Application() {
    MIST_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(MIST_BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() {
}

void Application::Run() {
    while (m_Running) {
        m_Window->OnUpdateStart();

        for (Layer* layer : m_LayerStack)
            layer->OnUpdateStart();

        for (Layer* layer : m_LayerStack)
            layer->OnUpdateEnd();

        //auto [x, y] = Input::GetMousePosition();
        //MIST_CORE_TRACE("{0}, {1}", x, y);

        m_Window->OnUpdateEnd();
    }
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(MIST_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(MIST_BIND_EVENT_FN(Application::OnWindowResize));

    for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin();) {
        (*--iter)->OnEvent(e);
        if (e.Handled)
            break;
    }

    // MIST_CORE_TRACE(e);
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return false;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
    m_Window->Resize(e.GetWidth(), e.GetHeight());
    return false;
}

} // namespace Mist