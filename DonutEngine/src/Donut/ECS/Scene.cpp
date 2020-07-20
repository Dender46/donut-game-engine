#include "dnpch.h"
#include "Scene.h"

namespace Donut {
	
	void Scene::AddEntity(Entity& entity)
	{
		m_Entities.push_back(entity);
		entity.m_ID = m_Entities.size();
		entity.m_SceneID = m_ID;
	}

}