#include "dnpch.h"

#include "Donut/Renderer/Renderer2D.h"
#include "Donut/Core/Random.h"
#include "ParticleSystem.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Donut {

	ParticleSystem::ParticleSystem(uint32_t maxParticles)
		: m_PoolIndex(maxParticles - 1)
	{
		m_ParticlePool.resize(maxParticles);
	}

	void ParticleSystem::OnUpdate(Timestep& ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;
			
			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}
			
			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			auto rotation = 100.0f * ts;
			particle.Rotation += rotation;
		}
	}

	void ParticleSystem::OnRender(OrthographicCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life		= particle.LifeRemaining;
			float size		= glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

			particle.LifeRemaining = life / particle.Lifetime;

			glm::vec3 position = { particle.Position.x, particle.Position.y, 0.8f };
			Renderer2D::DrawRotatedQuad(position, { size, size }, glm::radians(particle.Rotation), color);
		}

		Renderer2D::EndScene();
	}

	void ParticleSystem::Emit(const ParticleProps& props)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];

		// Lifetime
		particle.Lifetime = particle.LifeRemaining = props.Lifetime;
		particle.Active = true;
		
		// Position & Rotation
		particle.Position = props.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();
		
		// Velocity
		particle.Velocity = props.Velocity;
		particle.Velocity.x = props.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y = props.VelocityVariation.y * (Random::Float() - 0.5f);
		
		// Color
		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		// Size
		particle.SizeBegin = props.SizeBegin + props.SizeVariation * (float)(Random::Float() - 0.5);
		particle.SizeEnd = props.SizeEnd;
		
		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

}

