workspace "DonutEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "DonutEngine/vendor/GLFW/include"

include "DonutEngine/vendor/GLFW"

project "DonutEngine"
    location "DonutEngine"
    kind "SharedLib"
    language "C++"

    targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dnpch.h"
	pchsource "DonutEngine/src/dnpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
		"%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
    }

	links
	{
		"GLFW",
		"opengl32.lib"
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "DN_PLATFORM_WINDOWS",
            "DN_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "DN_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "DN_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "DN_DIST"
        optimize "On"
    

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "DonutEngine/vendor/spdlog/include",
        "DonutEngine/src"
    }

    links
    {
        "DonutEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "DN_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DN_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DN_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "DN_DIST"
        optimize "On"