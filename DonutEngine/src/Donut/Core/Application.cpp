#include "dnpch.h"

#include "Donut/Core/Application.h"
#include "Donut/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Donut {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		DN_PROFILE_FUNCTION();

		DN_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(DN_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	
	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		DN_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		DN_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		DN_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(DN_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(DN_BIND_EVENT_FN(Application::OnWindowResize));

		DN_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		DN_PROFILE_FUNCTION();

		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			{
				DN_PROFILE_SCOPE("Layers OnUpdate");

				if (!m_Minimized)
				{
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
				}
			}

			{
				DN_PROFILE_SCOPE("Layers OnImGuiRender");

				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			{
				DN_PROFILE_SCOPE("Window OnUpdate");

				m_Window->OnUpdate();
			}
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		DN_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			m_Minimized = true;

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}