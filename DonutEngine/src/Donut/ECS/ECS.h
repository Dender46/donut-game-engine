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
		EntityHandle MakeEntity(BaseECSComponent* components, const uint32_t* componentIDs, size_t numComponents);
		void RemoveEntity(EntityHandle handle);

		/* Component methods */
		template<typename Component>
		inline void AddComponent(EntityHandle handle, Component* component)
		{
			AddComponentInternal(handle, Component::ID, component);
		}

		// Which component shall be removed is determined by passed templated type name
		template<typename Component>
		inline bool RemoveComponent(EntityHandle handle)
		{
			return RemoveComponentInternal(handle, Component::ID);
		}
		
		template<typename Component>
		inline BaseECSComponent& GetComponent(EntityHandle handle)
		{
			return GetComponentInternal(handle, Component::ID);
		}
		
		/* System methods */
		inline void AddSystem(BaseECSSystem& system)
		{
			m_Systems.push_back(&system);
		}
		void UpdateSystems(Timestep ts);
		void RemoveSystem(BaseECSSystem& system);
	private:
		std::vector<BaseECSSystem*> m_Systems;
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

		inline std::vector<std::pair<uint32_t, uint32_t>> HandleToEntity(EntityHandle handle)
		{
			return HandleToRawType(handle)->second;
		}


		void AddComponentInternal(EntityHandle handle, uint32_t componentID, BaseECSComponent* component);
		// deletes component in m_Components
		void DeleteComponentInternal(uint32_t componentID, uint32_t componentIndex);
		// deletes component in m_Entities
		bool RemoveComponentInternal(EntityHandle handle, uint32_t componentID);
		BaseECSComponent* GetComponentInternal(EntityHandle handle, uint32_t componentID, std::vector<uint8_t>& componentsMemArray);

		void UpdateSystemWithMultipleTypes
		(
			size_t systemIndex, std::vector<uint32_t> systemTypes, std::vector<BaseECSComponent*>& componentsParam,
			std::vector<std::vector<uint8_t>*>& componentsMemArray, Timestep ts
		);

		size_t FindLeastCommonComponent(std::vector<uint32_t> systemTypes);
	};

}