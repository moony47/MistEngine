#include "mistpch.h"

#include "Application.h"

#include "Mist/Core/Logger.h"
#include "Mist/Renderer/Renderer.h"

namespace Mist {

Application* Application::s_Instance = nullptr;

Application::Application() :
    m_LayerStack(new LayerStack) {
    MIST_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = Scope<Window>(Window::Create());
    m_Window->SetEventCallback(MIST_BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();

    m_ImGuiLayer = std::make_shared<ImGuiLayer>();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {
}

void Application::Run() {
    while (m_Running) {
        DeltaTime deltaTime = m_Window->OnUpdate();

        // Logical Update
        if (m_RunInBackground || !m_Minimised) {
            for (Ref<Layer> layer : *m_LayerStack)
                layer->OnUpdate(deltaTime);
        }

        // Graphical update
        if (!m_Minimised) {
            m_Window->OnFrameStart(deltaTime);
            for (Ref<Layer> layer : *m_LayerStack)
                layer->OnFrameStart(deltaTime);
            for (Ref<Layer> layer : *m_LayerStack)
                layer->OnFrameEnd(deltaTime);
        }

        // GUI update
        m_ImGuiLayer->Begin();
        for (Ref<Layer> layer : *m_LayerStack)
            layer->OnImGuiRender(deltaTime);
        m_ImGuiLayer->End();

        // End of graphical update
        if (!m_Minimised) {
            m_Window->OnFrameEnd(deltaTime);
        }

        // Check if layer stack changed
        if (m_NewLayerStack) {
            delete m_LayerStack;
            m_LayerStack = m_NewLayerStack;
            m_NewLayerStack = nullptr;
        }
    }
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(MIST_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(MIST_BIND_EVENT_FN(Application::OnWindowResize));

    for (auto iter = m_LayerStack->end(); iter != m_LayerStack->begin();) {
        (*--iter)->OnEvent(e);
        if (e.Handled)
            break;
    }

    // MIST_CORE_TRACE(e);
}

void Application::PushLayer(Ref<Layer> layer) {
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PushLayer(layer);
}

void Application::PushOverlay(Ref<Layer> overlay) {
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PushOverlay(overlay);
}

void Application::PopLayer(Ref<Layer> layer) {
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PopLayer(layer);
}

void Application::PopOverlay(Ref<Layer> overlay) {
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PopOverlay(overlay);
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return false;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
    if (e.GetHeight() == 0 || e.GetWidth() == 0) {
        m_Minimised = true;
        return false;
    }

    m_Minimised = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    return false;
}

} // namespace Mist