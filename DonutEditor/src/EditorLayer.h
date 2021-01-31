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

		Scene m_Scene;
		Entity m_MainCamera;
		Entity m_ClipSizeCamera;
		bool m_UsingMainCamera = true;

		// Framebuffer and viewport specific
		Framebuffer::FramebufferProps m_FramebufferProps = { 640, 480 };
		Ref<Framebuffer> m_Framebuffer;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};

}