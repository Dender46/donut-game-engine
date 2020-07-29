#pragma once
#include "Donut/Core/Timestep.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	class BaseECSSystem
	{
	public:
		BaseECSSystem(const std::vector<uint32_t>& componentTypes) : m_ComponentTypes(componentTypes) {}
 		virtual void UpdateComponents(Timestep ts, BaseECSComponent** components);
		inline const std::vector<uint32_t>& GetComponentTypes() { return m_ComponentTypes; }
	private:
		std::vector<uint32_t> m_ComponentTypes;
	};

}