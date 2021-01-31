#pragma once

#include "Donut/ECS/ECS.h"
#include "Donut/Scene/Scene.h"

namespace Donut {
	
	class Entity
	{
	public:
		Entity(Scene* scene, EntityHandle handle);
		Entity(const Entity& other) = default;

		template<typename T>
		void AddComponent(const T& component)
		{
			DN_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			return m_Scene->m_ECS.AddComponent(m_Handle, component);
		}

		template<typename T>
		bool RemoveComponent()
		{
			DN_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this component!");
			return m_Scene->m_ECS.RemoveComponent<T>(m_Handle);
		}

		template<typename T>
		T& GetComponent()
		{
			DN_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this component!");
			return *m_Scene->m_ECS.GetComponent<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_ECS.HasComponent<T>(m_Handle);
		}

	private:
		EntityHandle m_Handle = nullptr;
		Scene* m_Scene = nullptr;
	};

}
