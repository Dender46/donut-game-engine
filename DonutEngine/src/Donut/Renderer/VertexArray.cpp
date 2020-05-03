#include "dnpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Donut\Platform\OpenGL\OpenGLVertexArray.h"

namespace Donut {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}