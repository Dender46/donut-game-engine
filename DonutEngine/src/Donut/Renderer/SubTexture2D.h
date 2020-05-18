#pragma once

#include <glm/glm.hpp>
#include "Donut/Renderer/Texture.h"

namespace Donut {

	class SubTexture2D
	{
	public:
		SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture()	const { return m_Texture; }
		const glm::vec2* GetTextureCoords()	const { return m_TextureCoords; }

		static Ref<SubTexture2D> CreateFromTexture(Ref<Texture2D> texture, const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& cellSize = {1.0f, 1.0f});
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TextureCoords[4];
	};

}