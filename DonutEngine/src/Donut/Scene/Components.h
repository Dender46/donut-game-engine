#pragma once
#include <glm/glm.hpp>

#include "Donut/ECS/ECS.h"
#include "Donut/Renderer/Camera.h"

namespace Donut {

	/// <summary>
	/// This is a list of components that are using ECS
	/// </summary>

	struct NameComponent : public ECSComponent<NameComponent>
	{
		std::string Name{ "Unnamed Entity" };

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string & name)
			: Name(name) {}
	};

	struct TransformComponent : public ECSComponent<TransformComponent>
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 & transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct ColorComponent : public ECSComponent<ColorComponent>
	{
		glm::vec4 Color{ 1.0f };

		ColorComponent() = default;
		ColorComponent(const ColorComponent&) = default;
		ColorComponent(const glm::vec4 & color)
			: Color(color) {}
	};

	struct CameraComponent : public ECSComponent<CameraComponent>
	{
		Donut::Camera Camera;
		bool IsPrimary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}
		CameraComponent(const glm::mat4& projection, bool isPrimary)
			: Camera(projection), IsPrimary(isPrimary) {}
	};

}
