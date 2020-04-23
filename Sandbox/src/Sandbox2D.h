#pragma once

#include <Donut.h>

class Sandbox2D : public Donut::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Donut::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Donut::Event& e) override;
private:
	Donut::OrthographicCameraController m_CameraController;

	Donut::Ref<Donut::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_BlueColor = { 0.2f, 0.3f, 0.9f, 1.0f };
};