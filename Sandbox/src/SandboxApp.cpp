#include <Donut.h>
#include <Donut/Core/EntryPoint.h>

#include "Sandbox2D.h"


class Sandbox : public Donut::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
};

Donut::Application* Donut::CreateApplication()
{
	return new Sandbox();
}