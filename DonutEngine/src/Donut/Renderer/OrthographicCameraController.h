#pragma once

#include "Donut/Renderer/Camera.h"
#include "Donut/Core/Timestep.h"
#include "Donut/Events/MouseEvent.h"
#include "Donut/Events/ApplicationEvent.h"

namespace Donut {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool canRotate = false);

		void OnUpdate(Timestep& ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio; // these members should be declared before m_Camera
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		bool m_CanRotate;
		float m_CameraMoveSpeed = 2.0f;
		float m_CameraRotationSpeed = 8.0f;
		float m_MinZoom = 1.0f;
		float m_MaxZoom = 10.0f;
		float m_ZoomSpeed = 0.5f;
	};

}
