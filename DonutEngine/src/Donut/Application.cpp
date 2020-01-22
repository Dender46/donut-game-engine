#include "dnpch.h"

#include "Application.h"

#include "Donut/Events/ApplicationEvent.h"
#include "Donut/Log.h"

namespace Donut {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}
	
	void Application::Run()
	{
		WindowResizedEvent e(1280, 720);
		DN_TRACE(e);

		while (true);
	}
}