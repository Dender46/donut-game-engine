#include <Donut.h>

class ExampleLayer : public Donut::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Donut::Input::IsKeyPressed(DN_KEY_TAB))
			DN_TRACE("Tab key is pressed!");
	}

	void OnEvent(Donut::Event& e) override
	{
		DN_TRACE("{0}", e);
	}
};

class Sandbox : public Donut::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Donut::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Donut::Application* Donut::CreateApplication()
{
	return new Sandbox();
}