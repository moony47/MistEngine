#include "mistpch.h"

#include "Application.h"

#include "Mist/Core/Logger.h"
#include "Mist/Renderer/Renderer.h"

namespace Mist {

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name) :
    m_LayerStack(new LayerStack) {
    MIST_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = Scope<Window>(Window::Create(WindowProps(name)));
    m_Window->SetEventCallback(MIST_BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();

    m_ImGuiLayer = std::make_shared<ImGuiLayer>();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {
}

void Application::Run() {
    MIST_PROFILE_FUNCTION();

    while (m_Running) {
        MIST_PROFILE_SCOPE("GameLoopIteration");

        DeltaTime deltaTime = m_Window->OnUpdate();

        // Logical Update
        if (m_RunInBackground || !m_Minimised) {
            MIST_PROFILE_SCOPE("OnUpdates");
            for (Ref<Layer> layer : *m_LayerStack)
                layer->OnUpdate(deltaTime);
        }

        // Rendering update
        if (!m_Minimised) {
            MIST_PROFILE_SCOPE("OnRenders");
            m_Window->OnFrameStart(deltaTime);
            for (Ref<Layer> layer : *m_LayerStack)
                layer->OnRender(deltaTime);
        }

        // GUI update
        {
            MIST_PROFILE_SCOPE("OnImGuiRenders");
            m_ImGuiLayer->Begin();
            for (Ref<Layer> layer : *m_LayerStack)
                layer->OnImGuiRender(deltaTime);
            m_ImGuiLayer->End();
        }

        if (!m_Minimised)
            m_Window->OnFrameEnd(deltaTime);

        // Check if layer stack changed
        if (m_NewLayerStack) {
            MIST_PROFILE_SCOPE("SwapLayerStack");
            delete m_LayerStack;
            m_LayerStack = m_NewLayerStack;
            m_NewLayerStack = nullptr;
        }
    }
}

void Application::Close() {
    m_Running = false;
}

void Application::OnEvent(Event& e) {
    MIST_PROFILE_FUNCTION();

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
    MIST_PROFILE_FUNCTION();
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> overlay) {
    MIST_PROFILE_FUNCTION();
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PushOverlay(overlay);
    overlay->OnAttach();
}

void Application::PopLayer(Ref<Layer> layer) {
    MIST_PROFILE_FUNCTION();
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PopLayer(layer);
}

void Application::PopOverlay(Ref<Layer> overlay) {
    MIST_PROFILE_FUNCTION();
    if (!m_NewLayerStack)
        m_NewLayerStack = new LayerStack(*m_LayerStack);
    m_NewLayerStack->PopOverlay(overlay);
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    Close();
    return true;
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