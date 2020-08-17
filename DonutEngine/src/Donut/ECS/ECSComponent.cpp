#include "dnpch.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* BaseECSComponent::m_ComponentTypes;

	uint32_t BaseECSComponent::RegisterComponent(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn, size_t size)
	{
		if (m_ComponentTypes == nullptr)
			m_ComponentTypes = new std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>();
		uint32_t index = (uint32_t)m_ComponentTypes->size();
		m_ComponentTypes->push_back({ createfn, freefn, size });
		return index;
	}

}