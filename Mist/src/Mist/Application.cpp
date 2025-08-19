#include "mistpch.h"

#include "Mist/Application.h"
#include "Mist/Input.h"
#include "Mist/Logger.h"

#include "Mist/Renderer/Renderer.h"
#include "Mist/Renderer/ShaderController.h"
#include "Mist/Renderer/Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/glm.hpp"

//TEMP
#include <GLFW/glfw3.h>

namespace Mist {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		MIST_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(MIST_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (m_Running) {
			float time = (float)glfwGetTime();
			DeltaTime deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Window->OnUpdateStart(deltaTime);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdateStart(deltaTime);
			for (Layer* layer : m_LayerStack)
				layer->OnUpdateEnd(deltaTime);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender(deltaTime);
			m_ImGuiLayer->End();

			m_Window->OnUpdateEnd(deltaTime);
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