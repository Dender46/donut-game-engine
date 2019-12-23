#pragma once

#ifdef DN_PLATFORM_WINDOWS

extern Donut::Application* Donut::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Donut::CreateApplication();
	app->Run();
	delete app;
}

#endif // DN_PLATFORM_WINDOWS

