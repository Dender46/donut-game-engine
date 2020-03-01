#pragma once

#include <glm/mat4x4.hpp>

namespace Donut {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		inline void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}
		inline const glm::vec3& GetPosition() { return m_Position; }

		inline void SetRotation(const float rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}
		inline const float GetRotation() { return m_Rotation; }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	public:
		glm::mat4 m_ProjectionMatrix, m_ViewMatrix, m_ViewProjectionMatrix;
		
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};

}