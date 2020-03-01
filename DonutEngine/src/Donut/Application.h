#pragma once

#include "Window.h"
#include "Donut/LayerStack.h"
#include "Donut/Events/Event.h"
#include "Donut/ImGui/ImGuiLayer.h"

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
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}
