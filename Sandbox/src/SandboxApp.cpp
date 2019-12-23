#include <Donut.h>

class Sandbox : public Donut::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Donut::Application* Donut::CreateApplication()
{
	return new Sandbox();
}