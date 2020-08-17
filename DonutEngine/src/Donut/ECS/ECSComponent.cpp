#include "dnpch.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* BaseECSComponent::s_ComponentTypes;

	uint32_t BaseECSComponent::RegisterComponent(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn, size_t size)
	{
		if (s_ComponentTypes == nullptr)
			s_ComponentTypes = new std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>();
		uint32_t index = (uint32_t)s_ComponentTypes->size();
		s_ComponentTypes->push_back({ createfn, freefn, size });
		return index;
	}

}