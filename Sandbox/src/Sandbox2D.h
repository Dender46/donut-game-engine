#pragma once

#include <Donut.h>

class Sandbox2D : public Donut::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnUpdate(Donut::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Donut::Event& e) override;
private:
	Donut::OrthographicCameraController m_CameraController;

	Donut::Ref<Donut::Texture2D> m_CheckerboardTexture, m_SpriteSheet;
	Donut::Ref<Donut::SubTexture2D> m_TextureStairs, m_TextureTree;

	b2World m_World;
	std::vector<Donut::Box> m_Bodies;

	glm::vec4 m_BlueColor = DN_COLOR_BLUE;
	float m_MouseX = 0.0f;
	float m_MouseY = 0.0f;
};