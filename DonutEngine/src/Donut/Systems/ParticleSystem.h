#pragma once

#include "Donut/Core/Timestep.h"
#include "Donut/Renderer/Camera.h"

#include <vector>
#include <glm/glm.hpp> // includes uint32_t

namespace Donut {

	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float Lifetime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t maxParticles);

		void OnUpdate(Timestep& ts);
		void OnRender(OrthographicCamera& camera);

		void Emit(const ParticleProps& particle);
	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation;
			float SizeBegin, SizeEnd;

			float Lifetime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex;
	};
}