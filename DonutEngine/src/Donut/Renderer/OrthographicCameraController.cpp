#include "dnpch.h"

#include "Donut/Renderer/OrthographicCameraController.h"
#include "Donut/Core/Input.h"
#include "Donut/Core/KeyCodes.h"

namespace Donut {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool canRotate)
		: m_AspectRatio(aspectRatio),
		  m_CanRotate(canRotate),
		  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep& ts)
	{
		DN_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(DN_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(DN_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (Input::IsKeyPressed(DN_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(DN_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (m_CanRotate)
		{
			if (Input::IsKeyPressed(DN_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(DN_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraMoveSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		DN_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DN_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DN_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		DN_PROFILE_FUNCTION();

		float zoom = e.GetYOffset() * m_ZoomSpeed;
		if (m_MinZoom <= m_ZoomLevel - zoom && m_ZoomLevel - zoom <= m_MaxZoom)
			m_ZoomLevel -= zoom;
		

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		DN_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}