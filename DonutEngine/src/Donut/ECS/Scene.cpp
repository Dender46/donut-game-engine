#include "dnpch.h"
#include "Scene.h"

#include "Donut/Renderer/Renderer2D.h"

namespace Donut {

	Scene::Scene()
	{
		m_SystemList.AddSystem(m_Renderer2DSystem);
	}

	void Scene::Update(Timestep ts)
	{
		m_ECS.UpdateSystems(m_SystemList, ts);
	}

}