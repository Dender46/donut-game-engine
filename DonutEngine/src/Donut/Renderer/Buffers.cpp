#include "dnpch.h"
#include "Buffers.h"

#include "Renderer.h"

#include "Donut\Platform\OpenGL\OpenGLBuffers.h"

namespace Donut {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(size, vertices);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(count, indices);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}