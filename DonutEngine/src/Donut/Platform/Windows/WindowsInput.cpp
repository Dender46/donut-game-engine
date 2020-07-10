#include "dnpch.h"

#include "Donut/Core/Application.h"
#include "Donut/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Donut {

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state  = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { float(xpos), float(ypos) };
	}

	float Input::GetMouseX()
	{
		auto[x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto[x, y] = GetMousePosition();
		return y;
	}

	std::pair<float, float> Input::GetRelativeMousePosition(const OrthographicCameraController& camera)
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

	float Input::GetRelativeMouseX(const OrthographicCameraController& camera)
	{
		auto [x, y] = GetRelativeMousePosition(camera);
		return x;
	}

	float Input::GetRelativeMouseY(const OrthographicCameraController& camera)
	{
		auto [x, y] = GetRelativeMousePosition(camera);
		return y;
	}
}