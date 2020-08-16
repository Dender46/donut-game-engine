#include "dnpch.h"
#include "ECS.h"

#include <cmath>

namespace Donut {

	ECS::~ECS()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); it++)
		{
			auto freeFn   = BaseECSComponent::GetFreeFunctionOfType(it->first);
			size_t typeSize = BaseECSComponent::GetSizeOfType(it->first);
			for (size_t i = 0; i < it->second.size(); i += typeSize)
				freeFn((BaseECSComponent*)&it->second[i]);

			for (auto entity : m_Entities)
				delete entity;
		}
	}

	EntityHandle ECS::MakeEntity(BaseECSComponent* components, const uint32_t* componentIDs, size_t numComponents)
	{
		std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>> * newEntity = new std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>();
		EntityHandle handle = (EntityHandle)newEntity;
		
		for (size_t i = 0; i < numComponents; i++)
		{
			auto createFn = BaseECSComponent::GetCreateFunctionOfType(componentIDs[i]);
			std::pair<uint32_t, uint32_t> newComponent;
			newComponent.first = componentIDs[i];
			newComponent.second = createFn(m_Components[componentIDs[i]], handle, &components[i]);
			newEntity->second.push_back(newComponent);
		}
		
		newEntity->first = (uint32_t)m_Entities.size();
		m_Entities.push_back(newEntity);

		return handle;
	}

	void ECS::RemoveEntity(EntityHandle handle)
	{
		std::vector<std::pair<uint32_t, uint32_t>>& entity = HandleToEntity(handle);
		for (size_t i = 0; i < entity.size(); i++)
			DeleteComponentInternal(entity[i].first, entity[i].second);

		size_t srcIndex = HandleToEntityIndex(handle);
		delete m_Entities[srcIndex];

		size_t destIndex = m_Entities.size() - 1;
		m_Entities[srcIndex] = m_Entities[destIndex];
		m_Entities[srcIndex]->first = (uint32_t)srcIndex; // this is important as we place last entity into place of old entity

		m_Entities.pop_back();
	}

	void ECS::UpdateSystems(Timestep ts)
	{
		std::vector<BaseECSComponent*> componentsParam;
		for (size_t i = 0; i < m_Systems.size(); i++)
		{
			std::vector<uint32_t> systemTypes = m_Systems[i]->GetComponentTypes();

			if (systemTypes.size() == 1)
			{
				size_t typeSize = BaseECSComponent::GetSizeOfType(systemTypes[0]);
				std::vector<uint8_t> memArray = m_Components[systemTypes[0]];

				for (size_t j = 0; j < memArray.size(); j += typeSize)
				{
					BaseECSComponent* component = (BaseECSComponent*)&memArray[j];
					m_Systems[i]->UpdateComponents(ts, &component);
				}
			}
			else
			{
				UpdateSystemWithMultipleTypes(i, systemTypes, componentsParam, ts);
			}
		}
	}

	void ECS::UpdateSystemWithMultipleTypes(size_t systemIndex, std::vector<uint32_t> systemTypes, std::vector<BaseECSComponent*>& componentsParam, Timestep ts)
	{
		componentsParam.resize(std::max(componentsParam.size(), systemTypes.size()));

		size_t typeSize = BaseECSComponent::GetSizeOfType(systemTypes[0]);
		std::vector<uint8_t> memArray = m_Components[systemTypes[0]];

		for (size_t i = 0; i < memArray.size(); i += typeSize)
		{
			componentsParam[0] = (BaseECSComponent*)&memArray[i];
			auto entityComponents = HandleToEntity(componentsParam[0]->Entity);

			bool isEntityValid = true; // does entity have desired components of types
			for (size_t j = 0; j < systemTypes.size(); j++)
			{
				if (j == 0) // to indicate that loop begins from second type
					continue;

				componentsParam[j] = GetComponentInternal(componentsParam[0]->Entity, systemTypes[j]);
				if (componentsParam[j] == nullptr)
				{
					isEntityValid = false;
					break;
				}
			}

			if (isEntityValid)
				m_Systems[systemIndex]->UpdateComponents(ts, &componentsParam[0]);
		}
	}

	void ECS::RemoveSystem(BaseECSSystem& system)
	{
		for (size_t i = 0; i < m_Systems.size(); i++)
			if (&system == m_Systems[i])
				m_Systems.erase(m_Systems.begin() + i);
	}

	void ECS::AddComponentInternal(EntityHandle handle, uint32_t componentID, BaseECSComponent* component)
	{
		auto& entity = HandleToEntity(handle);
		auto createFn = BaseECSComponent::GetCreateFunctionOfType(componentID);

		std::pair<uint32_t, uint32_t> newComponent;
		newComponent.first = componentID;
		newComponent.second = createFn(m_Components[componentID], handle, component);
		entity.push_back(newComponent);
	}

	void ECS::DeleteComponentInternal(uint32_t componentID, uint32_t componentIndex)
	{
		auto& memArray = m_Components[componentID];
		auto freeFn = BaseECSComponent::GetFreeFunctionOfType(componentID);
		size_t typeSize = BaseECSComponent::GetSizeOfType(componentID);
		
		size_t srcIndex = memArray.size() - typeSize; // index of last component
		BaseECSComponent* srcComp = (BaseECSComponent*)&memArray[srcIndex];
		BaseECSComponent* destComp = (BaseECSComponent*)&memArray[componentIndex];
		freeFn(destComp);

		// if asked component is last in array of components:
		if (srcIndex == componentIndex)
		{
			memArray.resize(srcIndex);
			return;
		}

		std::memcpy(destComp, srcComp, typeSize);
		auto entity = HandleToEntity(destComp->Entity);
		for (size_t i = 0; i < entity.size(); i++)
		{
			if (entity[i].first == componentID)
			{
				entity[i].second = componentIndex;
				break;
			}
		}

		memArray.resize(srcIndex);
	}

	bool ECS::RemoveComponentInternal(EntityHandle handle, uint32_t componentID)
	{
		auto entityComponents = HandleToEntity(handle);
		for (size_t i = 0; i < entityComponents.size(); i++)
		{
			if (entityComponents[i].first = componentID)
			{
				DeleteComponentInternal(componentID, entityComponents[i].second);
				size_t srcIndex  = entityComponents.size() - 1;
				size_t destIndex = i;
				entityComponents[destIndex] = entityComponents[srcIndex];
				entityComponents.pop_back();
				return true;
			}
		}
		return false;
	}

	BaseECSComponent* ECS::GetComponentInternal(EntityHandle handle, uint32_t componentID)
	{
		auto entityComponents = HandleToEntity(handle);
		for (size_t i = 0; i < entityComponents.size(); i++)
		{
			if (entityComponents[i].first = componentID)
				return (BaseECSComponent*)&m_Components[componentID][entityComponents[i].second];
		}
		return nullptr;
	}

}