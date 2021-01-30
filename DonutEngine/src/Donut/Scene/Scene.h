#pragma once

#include "Donut/ECS/ECS.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	class Scene
	{
	public:
		Scene()  = default;
		~Scene() = default;

		void AddSystem(BaseECSSystem& system);

		EntityHandle CreateEntity();

		template<class ...Components>
		EntityHandle CreateEntity(Components&... entitycomponents)
		{
			return m_ECS.MakeEntity(entitycomponents...);
		}

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
