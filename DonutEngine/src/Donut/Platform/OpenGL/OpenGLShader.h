#pragma once

#include "Donut/Renderer/Shader.h"
#include <glm/glm.hpp>

// Temporary, because its included in SandboxApp
typedef unsigned int GLenum;

namespace Donut {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string &path);
		OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, const int value);
		void UploadUniformFloat(const std::string& name, const float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2 value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3 value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4 value);

		void UploadUniformMat3(const std::string& name, const glm::mat3 matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4 matrix);
	private:
		std::string ReadFile(const std::string &path);
		std::unordered_map<GLenum, std::string> Parse(const std::string &source);
		void Compile(const std::unordered_map<GLenum, std::string> &shaderSources);
	private:
		std::string m_Path;
		uint32_t m_RendererID;
	};

}
