#pragma once

#include "Donut/Core/Core.h"
#include "Donut/Renderer/OrthographicCameraController.h"

namespace Donut {

	class  Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		static std::pair<float, float> GetRelativeMousePosition(const OrthographicCameraController& camera);
		static float GetRelativeMouseX(const OrthographicCameraController& camera);
		static float GetRelativeMouseY(const OrthographicCameraController& camera);
	};

}