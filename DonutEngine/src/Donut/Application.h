#pragma once

#include "Core.h"
#include "Window.h"
#include "Donut\LayerStack.h"
#include "Donut\Events\Event.h"
#include "Donut\Events\ApplicationEvent.h"

#include "Donut\Renderer\Shader.h"

#include "Donut\ImGui\ImGuiLayer.h"

namespace Donut {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}
