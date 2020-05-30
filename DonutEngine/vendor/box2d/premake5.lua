project "Box2D"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    
    targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "include/box2d/**.h",
        "src/collision/**.cpp",
        "src/dynamics/**.h",
        "src/dynamics/**.cpp",
        "src/common/**.cpp",
        "src/rope/**.cpp"
    }
    
    includedirs
    {
        "include",
        "src"
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"