#include "dnpch.h"

#include "Donut/Core/Application.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>

namespace Donut {

	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state  = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { float(xpos), float(ypos) };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return y;
	}

	std::pair<float, float> WindowsInput::GetRelativeMousePositionImpl(const OrthographicCameraController& camera)
	{
		auto [x, y] = Input::GetMousePosition();
		auto width = Application::Get().GetWindow().GetWidth();
		auto height = Application::Get().GetWindow().GetHeight();

		auto bounds = camera.GetBounds();
		auto pos = camera.GetCamera().GetPosition();

		float mouseX = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f + pos.x;
		float mouseY = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight() + pos.y;
		return { mouseX, mouseY };
	}

	float WindowsInput::GetRelativeMouseXImpl(const OrthographicCameraController& camera)
	{
		auto [x, y] = GetRelativeMousePositionImpl(camera);
		return x;
	}

	float WindowsInput::GetRelativeMouseYImpl(const OrthographicCameraController& camera)
	{
		auto [x, y] = GetRelativeMousePositionImpl(camera);
		return y;
	}
}