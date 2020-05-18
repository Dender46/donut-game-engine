#include "dnpch.h"
#include "Donut/Renderer/SubTexture2D.h"

namespace Donut {

	SubTexture2D::SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TextureCoords[0] = { min.x, min.y };
		m_TextureCoords[1] = { max.x, min.y };
		m_TextureCoords[2] = { max.x, max.y };
		m_TextureCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromTexture(Ref<Texture2D> texture, const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& cellSize)
	{
		glm::vec2 min = { coords.x * tileSize.x / texture->GetWidth(), coords.y * tileSize.y / texture->GetHeight() };
		glm::vec2 max = { (coords.x + cellSize.x) * tileSize.x / texture->GetWidth(), (coords.y + cellSize.y) * tileSize.y / texture->GetHeight() };

		return CreateRef<SubTexture2D>(texture, min, max);
	}
}