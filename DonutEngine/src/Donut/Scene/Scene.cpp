#include "dnpch.h"
#include "Donut/Scene/Scene.h"

namespace Donut {

	void Scene::AddSystem(BaseECSSystem& system)
	{
		bool isSystemValid = m_SystemList.AddSystem(system);
		DN_CORE_ASSERT(isSystemValid, "System is not valid!");
	}

	EntityHandle Scene::CreateEntity()
	{
		return m_ECS.MakeEntity();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_ECS.UpdateSystems(m_SystemList, ts);
	}

}