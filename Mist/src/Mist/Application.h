#pragma once
#include "Mist/Core.h"

#include "Mist/Events/EventSystem.h"
#include "Mist/ImGui/ImGuiLayer.h"
#include "Mist/LayerStack.h"
#include "Mist/Window.h"


namespace Mist {

	class Shader;
	class Texture2D;
	class VertexArray;
	class OrthographicCamera;

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