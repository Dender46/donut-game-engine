#pragma once
#include <glm/glm.hpp>

#include "Donut/ECS/ECS.h"

namespace Donut {

	/// <summary>
	/// This is a list of components that are using ECS
	/// </summary>

	struct NameComponent : public ECSComponent<NameComponent>
	{
		std::string name{ "Unnamed Entity" };
	};

	struct TransformComponent : public ECSComponent<TransformComponent>
	{
		glm::mat4 transform{ 1.0f };
	};

	struct ColorComponent : public ECSComponent<ColorComponent>
	{
		glm::vec4 color{ 1.0f };
	};

}
