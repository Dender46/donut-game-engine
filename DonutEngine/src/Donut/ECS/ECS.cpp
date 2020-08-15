#include "dnpch.h"
#include "ECS.h"

namespace Donut {

	ECS::~ECS()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); it++)
		{
			auto freeFn   = BaseECSComponent::GetFreeFunctionOfType(it->first);
			auto typeSize = BaseECSComponent::GetSizeOfType(it->first);
			for (int i = 0; i < it->second.size(); i += typeSize)
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
		
		newEntity->first = m_Entities.size();
		m_Entities.push_back(newEntity);

		return handle;
	}

	void ECS::RemoveEntity(EntityHandle handle)
	{
		std::vector<std::pair<uint32_t, uint32_t>>& entity = HandleToEntity(handle);
		for (size_t i = 0; i < entity.size(); i++)
			DeleteComponentInternal(entity[i].first, entity[i].second);

		auto srcIndex = HandleToEntityIndex(handle);
		delete m_Entities[srcIndex];

		auto destIndex = m_Entities.size() - 1;
		m_Entities[srcIndex] = m_Entities[destIndex];
		m_Entities[srcIndex]->first = srcIndex; // this is important as we place last entity into place of old entity

		m_Entities.pop_back();
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
		auto typeSize = BaseECSComponent::GetSizeOfType(componentID);
		
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

}

;