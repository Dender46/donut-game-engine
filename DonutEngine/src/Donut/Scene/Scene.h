#pragma once

#include "Donut/ECS/ECS.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		void AddSystem(BaseECSSystem& system)
		{
			bool isSystemValid = m_SystemList.AddSystem(system);
			DN_CORE_ASSERT(isSystemValid, "System is not valid!");
		}

		EntityHandle CreateEntity();

		template<typename Component>
		void AddComponent(EntityHandle entity, Component& component)
		{
			m_ECS.AddComponent(entity, component);
		}

		void OnUpdate(Timestep ts);

		ECS& getECS() { return m_ECS; }

	private:
		ECS m_ECS; // Lets create entities with given components
		ECSSystemList m_SystemList; // List of system that should be updated

	};

}
