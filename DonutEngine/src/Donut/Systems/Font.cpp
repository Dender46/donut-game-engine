#include "dnpch.h"
#include "Font.h"

namespace Donut {

    FT_Library Font::m_Lib;
    FT_Face Font::m_Face;
    std::map<char, Ref<Font::Character>> Font::m_Characters;

    void Font::Init()
    {
        DN_CORE_ASSERT(!FT_Init_FreeType(&m_Lib), "FREETYPE: Could not init FreeType Library");
    }

    void Font::LoadFont(const std::string& path, const uint32_t fontSize)
    {
        DN_CORE_ASSERT(!FT_New_Face(m_Lib, path.c_str(), 0, &m_Face), "FREETYPE: Failed to load font");
        DN_CORE_ASSERT(!FT_Set_Pixel_Sizes(m_Face, 0, fontSize),   "FREETYPE: Error setting pixel size");
        
        std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for (char ch : alphabet) {
            DN_CORE_ASSERT(!FT_Load_Char(m_Face, ch, FT_LOAD_RENDER), "FREETYPE: Error getting character");
            auto texture = Texture2D::Create(m_Face->glyph->bitmap.buffer, m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows);
            Character character = {
                texture,
                glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
                glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
                m_Face->glyph->advance.x
            };
            m_Characters[ch] = CreateRef<Character>(character);
        }
    }

    void Font::SetSize(const uint32_t size)
    {
        DN_CORE_ASSERT(!FT_Set_Pixel_Sizes(m_Face, 0, size), "FREETYPE: Error setting pixel size");
    }

    const Ref<Font::Character> Font::GetChar(const char ch)
    {
        DN_CORE_ASSERT(m_Characters.find(ch) != m_Characters.end(), "No available character exists")
        return m_Characters[ch];
    }

}