#include "dnpch.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	static uint32_t ComponentID = 0;

	uint32_t BaseECSComponent::NextID()
	{
		return ComponentID++;
	}

}