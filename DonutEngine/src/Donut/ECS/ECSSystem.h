#pragma once
#include "Donut/Core/Timestep.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	class BaseECSSystem
	{
	public:
		enum
		{
			FLAG_OPTIONAL = 1,
		};
		BaseECSSystem(const std::vector<uint32_t>& componentTypes) : m_ComponentTypes(componentTypes) {}
 		virtual void UpdateComponents(Timestep ts, BaseECSComponent** components);
		inline bool IsValid() const
		{
			for (size_t i = 0; i < m_ComponentFlags.size(); i++)
				if ((m_ComponentFlags[i] & FLAG_OPTIONAL) == 0)
					return true;

			return false;
		}

		inline const std::vector<uint32_t>& GetComponentTypes() const { return m_ComponentTypes; }
		inline const std::vector<uint32_t>& GetComponentFlags() const { return m_ComponentFlags; }

	protected:
		inline void AddComponent(uint32_t componentType, uint32_t componentFlag = 0)
		{
			m_ComponentTypes.push_back(componentType);
			m_ComponentFlags .push_back(componentFlag);
		}
	private:
		std::vector<uint32_t> m_ComponentTypes;
		std::vector<uint32_t> m_ComponentFlags;
	};

}