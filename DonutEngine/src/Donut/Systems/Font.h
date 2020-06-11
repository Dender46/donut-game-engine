#pragma once

#include <iostream>
#include <ft2build.h>

#include FT_FREETYPE_H

namespace Donut {

    class Font
	{
	public:
        static void LoadFont(const std::string& path);
	};

}
