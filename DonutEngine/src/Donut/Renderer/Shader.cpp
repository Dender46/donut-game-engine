#include "dnpch.h"

#include "Renderer.h"
#include "Shader.h"

#include "Donut/Platform/OpenGL/OpenGLShader.h"

namespace Donut {

	Ref<Shader> Shader::Create(const std::string &path)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(path);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string & vertexSrc, const std::string & fragmentSrc)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
