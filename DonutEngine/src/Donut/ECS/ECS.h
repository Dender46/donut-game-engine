#pragma once
#include <map>

#include "Donut/ECS/ECSSystem.h"
#include "Donut/ECS/ECSComponent.h"

namespace Donut {

	class ECS
	{
	public:
		ECS() = default;
		~ECS();

		/* Entity methods */
		EntityHandle MakeEntity();
		EntityHandle MakeEntity(BaseECSComponent** components, const uint32_t* componentIDs, size_t numComponents);
		void RemoveEntity(EntityHandle handle);
		
		template<class ...Components>
		EntityHandle MakeEntity(Components&... entitycomponents)
		{
			BaseECSComponent* components[] = { &entitycomponents... };
			uint32_t componentIDs[] = { Components::ID... };
			return MakeEntity(components, componentIDs, sizeof...(Components));
		}

		/* Component methods */
		template<typename Component>
		inline void AddComponent(EntityHandle handle, const Component& component)
		{
			AddComponentInternal(handle, HandleToEntity(handle), Component::ID, component);
		}

		template<typename Component>
		inline bool RemoveComponent(EntityHandle handle)
		{
			return RemoveComponentInternal(handle, Component::ID);
		}
		
		template<typename Component>
		inline Component* GetComponent(EntityHandle handle)
		{
			return (Component*)GetComponentInternal(handle, Component::ID, m_Components[Component::ID]);
		}

		template<typename Component>
		inline bool HasComponent(EntityHandle handle)
		{
			return GetComponentInternal(handle, Component::ID, m_Components[Component::ID]) != nullptr;
		}
		
		template<typename Component>
		inline std::vector<Component*> GetComponentsOfType()
		{
			size_t componentsCount = m_Components[Component::ID].size() / Component::SIZE;
			std::vector<Component*> components;
			components.resize(componentsCount);

			size_t index = 0;
			for (size_t i = 0; i < m_Components[Component::ID].size(); i += Component::SIZE)
				components[index++] = (Component*)&m_Components[Component::ID][i];

			return components;
		}

		/* System method */
		void UpdateSystems(ECSSystemList& systems, Timestep ts);
	private:
		// uint32_t - ID of components type
		// uint8_t in vector - because we don't know type of components we use general size of component
		std::map<uint32_t, std::vector<uint8_t>> m_Components;
		// Vector consists of entities, which consist of 
		// {index_in_array_of_entities, [ {ID/type, index_in_m_Components}, ... ]}
		// Pointers to pairs allows for shuffling of vector
		std::vector<std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t> > >* > m_Entities;


		inline std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* HandleToRawType(EntityHandle handle)
		{
			return (std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>*)handle;
		}

		inline uint32_t HandleToEntityIndex(EntityHandle handle)
		{
			return HandleToRawType(handle)->first;
		}

		// If we need to modify entity's component list (add/remove) we should return reference here
		inline std::vector<std::pair<uint32_t, uint32_t>>& HandleToEntity(EntityHandle handle)
		{
			return HandleToRawType(handle)->second;
		}


		void AddComponentInternal(EntityHandle handle, std::vector<std::pair<uint32_t, uint32_t> >& entityComponents, uint32_t componentID, const BaseECSComponent& component);
		// deletes component in m_Components
		void DeleteComponentInternal(uint32_t componentID, uint32_t componentIndex);
		// deletes component in m_Entities
		bool RemoveComponentInternal(EntityHandle handle, uint32_t componentID);
		BaseECSComponent* GetComponentInternal(EntityHandle handle, uint32_t componentID, std::vector<uint8_t>& componentsMemArray);

		void UpdateSystemWithMultipleTypes
		(
			ECSSystemList& systems, size_t systemIndex, std::vector<uint32_t> systemTypes, std::vector<BaseECSComponent*>& componentsParam,
			std::vector<std::vector<uint8_t>*>& componentsMemArray, Timestep ts
		);

		size_t FindLeastCommonComponent(const std::vector<uint32_t>& systemTypes, const std::vector<uint32_t>& systemFlags);
	};

}