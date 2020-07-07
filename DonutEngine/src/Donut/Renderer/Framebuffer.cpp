#include "dnpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Donut/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Donut {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferProps& props)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(props);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}