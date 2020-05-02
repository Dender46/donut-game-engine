#pragma once

#ifdef DN_PLATFORM_WINDOWS

extern Donut::Application* Donut::CreateApplication();

int main(int argc, char** argv)
{
	Donut::Log::Init();

	DN_PROFILE_SESSION_START("start", "DonutProfile-start.log");
	auto app = Donut::CreateApplication();
	DN_PROFILE_SESSION_END();

	DN_PROFILE_SESSION_START("update", "DonutProfile-update.log");
	app->Run();
	DN_PROFILE_SESSION_END();

	DN_PROFILE_SESSION_START("shutdown", "DonutProfile-shutdown.log");
	delete app;
	DN_PROFILE_SESSION_END();
}

#endif // DN_PLATFORM_WINDOWS

