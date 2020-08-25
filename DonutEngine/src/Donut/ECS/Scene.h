#pragma once
#include "Donut/ECS/ECS.h"
#include "Donut/ECS/Components/ECSComponents.h"
#include "Donut/ECS/Systems/ECSSystems.h"

#include "Donut/Renderer/OrthographicCameraController.h"

namespace Donut {

	class Scene
	{
	public:
		Scene();
		void Update(Timestep ts);

		template<class... Components>
		EntityHandle MakeEntity(Components&&... entitycomponents)
		{
			return m_ECS.MakeEntity(entitycomponents...);
		}

	private:
		ECSSystemList m_SystemList;
		
		Renderer2DSystem m_Renderer2DSystem;

		ECS m_ECS;
	};

}
