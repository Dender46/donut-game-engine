#pragma once
#include "Donut/Core/Timestep.h"
#include "Donut/ECS/ECS.h"

namespace Donut {

	class Renderer2DSystem : public BaseECSSystem
	{
	public:
		Renderer2DSystem();
		virtual void UpdateComponents(Timestep ts, BaseECSComponent** components);
	};

}
