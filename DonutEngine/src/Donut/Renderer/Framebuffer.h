#pragma once

namespace Donut {

	class Framebuffer
	{
	public:
		struct FramebufferProps
		{
			uint32_t Width, Height;
			uint32_t Samples = 1;

			bool SwapChainTarget = false;
		};

		virtual ~Framebuffer() = default;
		static Ref<Framebuffer> Create(const FramebufferProps& props);

		virtual void Invalidate() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual const FramebufferProps& GetProperties() const = 0;
	};

}