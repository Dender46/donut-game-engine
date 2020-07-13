#pragma once

#include "Donut/Renderer/Framebuffer.h"

namespace Donut {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferProps& props);
		virtual ~OpenGLFramebuffer();
		virtual void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual inline uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; };
		virtual inline const FramebufferProps& GetProperties() const override { return m_Props; };
	private:
		FramebufferProps m_Props;
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
	};

}