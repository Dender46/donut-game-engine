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
		BaseECSSystem() = default;
		virtual void UpdateComponents(Timestep ts, BaseECSComponent** components);
		bool IsValid() const;

		inline const std::vector<uint32_t>& GetComponentTypes() const { return m_ComponentTypes; }
		inline const std::vector<uint32_t>& GetComponentFlags() const { return m_ComponentFlags; }

	protected:
		inline void AddComponent(uint32_t componentType, uint32_t componentFlag = 0)
		{
			m_ComponentTypes.push_back(componentType);
			m_ComponentFlags.push_back(componentFlag);
		}
	private:
		std::vector<uint32_t> m_ComponentTypes;
		std::vector<uint32_t> m_ComponentFlags;
	};


	class ECSSystemList
	{
	public:
		ECSSystemList() = default;

		void RemoveSystem(BaseECSSystem& system);
		bool AddSystem(BaseECSSystem& system);
		
		inline size_t size() const { return m_Systems.size(); }
		inline BaseECSSystem* operator[](size_t index) const { return m_Systems[index]; }
	private:
		std::vector<BaseECSSystem*> m_Systems;
	};
}