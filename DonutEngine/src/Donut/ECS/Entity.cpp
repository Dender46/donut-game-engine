#include "dnpch.h"
#include "Entity.h"

namespace Donut {
	
	Entity::Entity(const std::string name)
		: m_ID(0), m_SceneID(0), m_Name(name)
	{
		
	}

	template<typename T>
	void Entity::AddComponent(T component)
	{

	}
}