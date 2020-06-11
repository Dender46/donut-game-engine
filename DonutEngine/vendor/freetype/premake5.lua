project "freetype"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "FT_DEBUG_LEVEL_ERROR",
        "FT_DEBUG_LEVEL_TRACE",
        "FT2_BUILD_LIBRARY"
    }

    files
    {
        "include/**.h",
        "src/base/ftsystem.c",
        "src/base/ftinit.c",
        "src/base/ftdebug.c",

        "src/base/ftbase.c",
        
        "src/base/ftbbox.c",
        "src/base/ftglyph.c",
        "src/base/ftbitmap.c",        -- ftglyph.c needs ftbitmap.c

        -- font drivers (optional; at least one is needed)
        "src/bdf/bdf.c",              -- BDF font driver 
        "src/cff/cff.c",            -- CFF/OpenType font driver
        "src/cid/type1cid.c",       -- Type 1 CID-keyed font driver
        "src/pcf/pcf.c",            -- PCF font driver
        "src/pfr/pfr.c",            -- PFR/TrueDoc font driver
        "src/sfnt/sfnt.c",          -- SFNT files support
                                    --(TrueType & OpenType)
        "src/truetype/truetype.c",  -- TrueType font driver
        "src/type1/type1.c",        -- Type 1 font driver
        "src/type42/type42.c",      -- Type 42 font driver
        "src/winfonts/winfnt.c",     -- Windows FONT / FNT font driver

        -- rasterizers (optional; at least one is needed for vector formats)

        "src/raster/raster.c",     -- monochrome rasterizer
        "src/smooth/smooth.c",     -- anti-aliasing rasterizer
    
        -- auxiliary modules (optional)
    
        "src/autofit/autofit.c",   -- auto hinting module
        "src/cache/ftcache.c",     -- cache sub-system (in beta)
        "src/gzip/ftgzip.c",       -- support for compressed fonts (.gz)
        "src/lzw/ftlzw.c",         -- support for compressed fonts (.Z)
        "src/bzip2/ftbzip2.c",     -- support for compressed fonts (.bz2)
        "src/gxvalid/gxvalid.c",   -- TrueTypeGX/AAT table validation
        "src/otvalid/otvalid.c",   -- OpenType table validation
        "src/psaux/psaux.c",       -- PostScript Type 1 parsing
        "src/pshinter/pshinter.c", -- PS hinting module
        "src/psnames/psnames.c"   -- PostScript glyph names support
    }
    
    includedirs
    {
        "include"
    }

    filter "system:windows"
        buildoptions "/MP"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"