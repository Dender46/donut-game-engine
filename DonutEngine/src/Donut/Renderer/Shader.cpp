#include "dnpch.h"

#include "Renderer.h"
#include "Shader.h"

#include "Donut/Platform/OpenGL/OpenGLShader.h"

namespace Donut {

	Ref<Shader> Shader::Create(const std::string& path)
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

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::CurrentAPI())
		{
		case RendererAPI::API::None:
			DN_CORE_ASSERT(false, "RendererAPI::None is currently unsupported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
			break;
		}

		DN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		DN_PROFILE_FUNCTION();

		auto& name = shader->GetName();
		DN_CORE_ASSERT(!Exists(name), "Shader already exists with such name!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		DN_PROFILE_FUNCTION();

		DN_CORE_ASSERT(!Exists(name), "Shader already exists with such name!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		DN_PROFILE_FUNCTION();

		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		DN_PROFILE_FUNCTION();

		DN_CORE_ASSERT(Exists(name), "No shader with such name!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}
