#pragma once

#include <glm/glm.hpp>
#include <box2d/box2d.h>
#include "Donut/Renderer/Renderer2D.h"

namespace Donut {

	class Box
	{
	public:
		Box(b2World* world, b2FixtureDef* phProps, glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color);
		Box(b2World* world, b2PolygonShape* shape, float density, glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color);

		void Draw();
	private:
		void Init(b2World* world, b2FixtureDef* phProps, b2Vec2 position, float rotation);

		glm::vec3 m_Position;

		glm::vec2 m_Size;
		glm::vec4 m_Color;

		b2Body* m_Body;
	};

}