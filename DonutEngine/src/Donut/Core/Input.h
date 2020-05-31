#pragma once

#include "Donut/Core/Core.h"
#include "Donut/Renderer/OrthographicCameraController.h"

namespace Donut {

	class  Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		inline static std::pair<float, float> GetRelativeMousePosition(const OrthographicCameraController& camera) { return s_Instance->GetRelativeMousePositionImpl(camera); }
		inline static float GetRelativeMouseX(const OrthographicCameraController& camera) { return s_Instance->GetRelativeMouseXImpl(camera); }
		inline static float GetRelativeMouseY(const OrthographicCameraController& camera) { return s_Instance->GetRelativeMouseYImpl(camera); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		virtual std::pair<float, float> GetRelativeMousePositionImpl(const OrthographicCameraController& camera) = 0;
		virtual float GetRelativeMouseXImpl(const OrthographicCameraController& camera) = 0;
		virtual float GetRelativeMouseYImpl(const OrthographicCameraController& camera) = 0;
	private:
		static Scope<Input> s_Instance;
	};

}