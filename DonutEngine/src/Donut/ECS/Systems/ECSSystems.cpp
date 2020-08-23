#include "dnpch.h"
#include "Donut/ECS/Components/ECSComponents.h"
#include "Donut/ECS/Systems/ECSSystems.h"

#include "Donut/Renderer/Renderer2D.h"

namespace Donut {

	Renderer2DSystem::Renderer2DSystem() : BaseECSSystem()
	{
		AddComponent(TransformComponent::ID);
		AddComponent(ColorComponent::ID);
		AddComponent(ColorComponent::ID);
	}

	void Renderer2DSystem::UpdateComponents(Timestep ts, BaseECSComponent** components)
	{
		TransformComponent* t = (TransformComponent*)components[0];
		ColorComponent* c = (ColorComponent*)components[1];

		Renderer2D::DrawQuad(t->transform, c->color);
	}



}