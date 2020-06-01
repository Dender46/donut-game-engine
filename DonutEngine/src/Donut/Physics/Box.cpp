#include "dnpch.h"
#include "Box.h"

namespace Donut {

	Box::Box(b2World* world, b2FixtureDef* phProps, glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color)
		: m_Position(position), m_Size(size), m_Color(color)
	{
		Init(world, phProps, { m_Position.x, m_Position.y }, rotation);
	}

	Box::Box(b2World* world, b2PolygonShape* shape, float density, glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color)
		: m_Position(position), m_Size(size), m_Color(color)
	{
		b2FixtureDef def;
		def.shape = shape;
		def.density = density;

		Init(world, &def, { m_Position.x, m_Position.y }, rotation);
	}

	void Box::Init(b2World* world, b2FixtureDef* phProps, b2Vec2 position, float rotation)
	{
		b2BodyDef bodyDef;
		if (phProps->density > 0)
			bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);
		m_Body = world->CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(m_Size.x, m_Size.y);

		phProps->shape = &dynamicBox;
		m_Body->CreateFixture(phProps);
	}

	void Box::Draw()
	{
		auto position = m_Body->GetPosition();
		auto rotation = m_Body->GetAngle();
		Renderer2D::DrawRotatedQuad({ position.x, position.y / 2, m_Position.z }, m_Size, rotation, m_Color);
	}
}