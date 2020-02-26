#include "dnpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Donut\Platform\OpenGL\OpenGLVertexArray.h"

namespace Donut {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}