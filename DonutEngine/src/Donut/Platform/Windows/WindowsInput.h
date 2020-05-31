#pragma once

#include "Donut/Core/Input.h"

namespace Donut {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

		virtual std::pair<float, float> WindowsInput::GetRelativeMousePositionImpl(const OrthographicCameraController& camera) override;
		virtual float GetRelativeMouseXImpl(const OrthographicCameraController& camera) override;
		virtual float GetRelativeMouseYImpl(const OrthographicCameraController& camera) override;
	};

}