#pragma once

#include "Core.h"
#include "Window.h"
#include "Donut\LayerStack.h"
#include "Donut\Events\Event.h"
#include "Donut\Events\ApplicationEvent.h"

namespace Donut {

	class DONUT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in client
	Application* CreateApplication();
}
