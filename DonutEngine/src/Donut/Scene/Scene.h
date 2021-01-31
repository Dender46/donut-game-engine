#pragma once

#include "Donut/ECS/ECS.h"

namespace Donut {

	class Entity;

	class Scene
	{
	public:
		Scene()  = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());

		void AddSystem(BaseECSSystem& system);
		void OnUpdate(Timestep ts);

		ECS& getECS() { return m_ECS; }

	private:
		ECS m_ECS; // Lets create entities with given components
		ECSSystemList m_SystemList; // List of system that should be updated

		friend class Entity;
	};

}
