#include "dnpch.h"
#include "Donut/Scene/Scene.h"
#include "Donut/Scene/Components.h"
#include "Donut/Scene/Entity.h"

namespace Donut {

	void Scene::AddSystem(BaseECSSystem& system)
	{
		bool isSystemValid = m_SystemList.AddSystem(system);
		DN_CORE_ASSERT(isSystemValid, "System is not valid!");
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity newEntity = { this, m_ECS.MakeEntity() };
		newEntity.AddComponent(NameComponent(name.empty() ? "Unnamed Entity" : name));
		newEntity.AddComponent(TransformComponent());
		return newEntity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_ECS.UpdateSystems(m_SystemList, ts);
	}

}