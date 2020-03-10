#include "dnpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Donut {
	
	GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		
		DN_CORE_ASSERT(false, "Uknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string & path)
	{
		m_Path = path;

		const std::string& source = ReadFile(path);
		auto shaderSources = Parse(source);
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	std::string OpenGLShader::ReadFile(const std::string &path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);	//move pointer to end
			result.resize(in.tellg());	//set size of result string
			in.seekg(0, std::ios::beg);	//move pointer to begin
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			DN_CORE_ERROR("Could not open file at: {0}", path);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::Parse(const std::string &source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			DN_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin); // getting string of type
			DN_CORE_ASSERT(ShaderTypeFromString(type), "Invalid type of shader!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos); // looking for next shader type
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> &shaderSources)
	{
		m_RendererID = glCreateProgram();
		std::vector<GLuint> glShaderIDs(shaderSources.size());

		for (auto& shaderData : shaderSources)
		{
			GLenum shaderType = shaderData.first;
			const GLchar *source = shaderData.second.c_str();

			// Create an empty shader handle
			GLuint shader = glCreateShader(shaderType);

			// Send the shader source code to GL
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);
				
				DN_CORE_ERROR("Failed at type {0}:\n{1}", shaderType, infoLog.data());
				DN_CORE_ASSERT(false, "Shader compilation error!");
				return;
			}

			// Vertex and fragment shaders are successfully compiled.
			// Now time to link them together into a program.

			// Attach our shaders to our program
			glAttachShader(m_RendererID, shader);
			glShaderIDs.push_back(shader);
		}

		
		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			for (GLuint id : glShaderIDs)
				glDeleteShader(id);

			DN_CORE_ERROR("{0}", infoLog.data());
			DN_CORE_ASSERT(false, "Shader linking failed!");
			return;
		}

		for (GLuint id : glShaderIDs)
			glDetachShader(m_RendererID, id);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string & name, const int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		DN_CORE_ASSERT(location != -1, name + " - does not correspond to an active uniform variable in program!");

		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string & name, const float value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		DN_CORE_ASSERT(location != -1, name + " - does not correspond to an active uniform variable in program!");

		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string & name, const glm::vec2 value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		DN_CORE_ASSERT(location != -1, name + " - does not correspond to an active uniform variable in program!");

		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string & name, const glm::vec3 value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		DN_CORE_ASSERT(location != -1, name + " - does not correspond to an active uniform variable in program!");

		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string & name, const glm::vec4 value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		DN_CORE_ASSERT(location != -1, name + " - does not correspond to an active uniform variable in program!");

		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string & name, const glm::mat3 matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		DN_CORE_ASSERT(location != -1, name + " - does not correspond to an active uniform variable in program!");

		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4 matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		DN_CORE_ASSERT(location != -1, name + " - does not correspond to an active uniform variable in program!");

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}
