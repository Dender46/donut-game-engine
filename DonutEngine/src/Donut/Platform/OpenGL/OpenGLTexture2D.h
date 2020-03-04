#pragma once

#include "Donut\Renderer\Texture.h"

namespace Donut {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		inline virtual const uint32_t GetWidth() const override { return m_Width; };
		inline virtual const uint32_t GetHeight() const override { return m_Height; };

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t m_RendererID;

		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
	};

}