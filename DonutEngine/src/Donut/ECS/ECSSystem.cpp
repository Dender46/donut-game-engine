#include "dnpch.h"
#include "ECSSystem.h"

namespace Donut {

	bool BaseECSSystem::IsValid() const
	{
		for (size_t i = 0; i < m_ComponentFlags.size(); i++)
			if ((m_ComponentFlags[i] & FLAG_OPTIONAL) == 0)
				return true;

		return false;
	}

	void BaseECSSystem::UpdateComponents(Timestep ts, BaseECSComponent** components)
	{
		// This should be overriden by child class
	}


	bool ECSSystemList::AddSystem(BaseECSSystem& system)
	{
		if (!system.IsValid())
			return false;

		m_Systems.push_back(&system);
		return true;
	}

	void ECSSystemList::RemoveSystem(BaseECSSystem& system)
	{
		for (size_t i = 0; i < m_Systems.size(); i++)
			if (&system == m_Systems[i])
				m_Systems.erase(m_Systems.begin() + i);
	}
}