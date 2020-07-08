#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "Donut/ImGui/ImGuiLayer.h"

#include "Donut/Events/ApplicationEvent.h"
#include "Donut/Events/Event.h"

namespace Donut {

	class Application
	{
	public:
		Application(const std::string& name = "Application Name");
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}
