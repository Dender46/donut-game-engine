#include "dnpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Donut\Platform\OpenGL\OpenGLTexture2D.h"

namespace Donut {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(void* data, uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(data, width, height, internalFormat, format);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
