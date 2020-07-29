#pragma once

#include <Donut.h>

namespace Donut {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		OrthographicCameraController m_CameraController;

		// Framebuffer and viewport specific
		Framebuffer::FramebufferProps m_FramebufferProps = { 640, 480 };
		Ref<Framebuffer> m_Framebuffer;
		bool m_ViewportFocused, m_ViewportHovered;

		// Textures
		Ref<Texture2D> m_CheckerboardTexture, m_SpriteSheet;
		Ref<SubTexture2D> m_TextureStairs, m_TextureTree;

		// Particles
		ParticleSystem m_ParticleSystem;
		ParticleProps m_ParticleProps;

		// Physics
		b2World m_World;
		std::vector<Box> m_Bodies;

		float m_MouseX = 0.0f;
		float m_MouseY = 0.0f;
	};

}