#pragma once

#include "Core.h"
#include "Window.h"
#include "Donut\Input.h"
#include "Donut\LayerStack.h"

#include "Donut\Events\Event.h"
#include "Donut\Events\KeyEvent.h"
#include "Donut\Events\MouseEvent.h"
#include "Donut\Events\ApplicationEvent.h"

#include "Donut\Renderer\Shader.h"
#include "Donut\Renderer\Buffers.h"
#include "Donut\Renderer\VertexArray.h"
#include "Donut\Renderer\RenderCommand.h"
#include "Donut\Renderer\Renderer.h"
#include "Donut\Renderer\Camera.h"

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
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}
