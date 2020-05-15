#pragma once

#include "Donut\Renderer\Texture.h"

#include <glad\glad.h>

namespace Donut {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		inline virtual const uint32_t GetWidth() const override { return m_Width; };
		inline virtual const uint32_t GetHeight() const override { return m_Height; };

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void SetData(void* data, uint32_t size) override;

		inline virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		uint32_t m_RendererID;

		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		
		GLenum m_StorageFormat, m_DataFormat;
	};

}