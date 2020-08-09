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


		return handle;
	}

	void ECS::RemoveEntity(EntityHandle entity)
	{
	}

}

