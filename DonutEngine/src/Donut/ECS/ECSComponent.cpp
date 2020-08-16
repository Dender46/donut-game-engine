#include "dnpch.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	uint32_t BaseECSComponent::RegisterComponent(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn, size_t size)
	{
		uint32_t index = (uint32_t)m_ComponentTypes.size();
		m_ComponentTypes.push_back({ createfn, freefn, size });
		return index;
	}

}