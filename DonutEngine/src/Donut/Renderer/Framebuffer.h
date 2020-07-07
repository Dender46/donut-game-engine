#pragma once

namespace Donut {

	class Framebuffer
	{
	public:
		struct FramebufferProps
		{
			float Width, Height;
		};

		static Ref<Framebuffer> Create(const FramebufferProps& props);

		virtual void Invalidate() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual const FramebufferProps& GetProperties() const = 0;
	};

}