#include "dnpch.h"
#include "Donut/Scene/Scene.h"

namespace Donut {

	Scene::~Scene()
	{
		
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