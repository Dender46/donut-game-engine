#pragma once

#include <iostream>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "glm/vec2.hpp"
#include "Donut/Renderer/Texture.h"

namespace Donut {

    class Font
	{
	public:
		static void Init();
        static void LoadFont(const std::string& path, const uint32_t fontSize);
		static void SetSize(const uint32_t size);

		struct Character {
			Ref<Texture2D> Texture; // ID handle of the glyph texture
			glm::ivec2 Size;		// Size of glyph
			glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
			uint32_t Advance;   // Offset to advance to next glyph
		};

		static const Ref<Character> GetChar(const char ch);

	private:
		static FT_Library m_Lib;
		static FT_Face m_Face;
		
		static std::map<char, Ref<Character>> m_Characters;
	};

}
