#pragma once

#include <string>
#include <vector>

#include "Donut/ECS/Entity.h"

namespace Donut {

	class Scene
	{
	public:
		Scene(const std::string& name = "Scene")
			: m_ID(0), m_Name(name) {};

		void AddEntity(Entity& entity);
	private:
		uint32_t m_ID;
		std::string m_Name;
		std::vector<uint32_t> m_Entities;
	};

}