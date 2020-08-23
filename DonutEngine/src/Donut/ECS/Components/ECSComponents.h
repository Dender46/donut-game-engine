#pragma once
#include <glm/glm.hpp>

#include "Donut/ECS/ECS.h"

namespace Donut {

	struct TransformComponent : public ECSComponent<TransformComponent>
	{
		glm::mat4 transform;
	};

	struct ColorComponent : public ECSComponent<ColorComponent>
	{
		glm::vec4 color;
	};


}
