#pragma once

#include <Donut.h>

namespace Donut {

	struct TransformComponent : public ECSComponent<TransformComponent>
	{
		TransformComponent(const glm::mat4& transform) : transform(transform) {}
		glm::mat4 transform;
	};

	struct ColorComponent : public ECSComponent<ColorComponent>
	{
		ColorComponent(const glm::vec4& color) : color(color) {}
		glm::vec4 color;
	};

	class Renderer2DSystem : public BaseECSSystem
	{
	public:
		Renderer2DSystem();

		virtual void UpdateComponents(Timestep ts, BaseECSComponent** components);
	};

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

		// ECS
		ECS m_ECS;
		EntityHandle m_Entity = nullptr;
		Renderer2DSystem m_Renderer2DSystem;
		ECSSystemList m_SystemList;

		// Framebuffer and viewport specific
		Framebuffer::FramebufferProps m_FramebufferProps = { 640, 480 };
		Ref<Framebuffer> m_Framebuffer;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

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