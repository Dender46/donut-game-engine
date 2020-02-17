workspace "DonutEngine"
    architecture "x64"
	startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]  = "DonutEngine/vendor/GLFW/include"
IncludeDir["Glad"]  = "DonutEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "DonutEngine/vendor/imgui"
IncludeDir["glm"]   = "DonutEngine/vendor/glm"

group "Dependencies"
	include "DonutEngine/vendor/GLFW"
	include "DonutEngine/vendor/Glad"
	include "DonutEngine/vendor/imgui"
group ""

project "DonutEngine"
    location "DonutEngine"
    kind "SharedLib"
    language "C++"
	staticruntime "off"

    targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dnpch.h"
	pchsource "DonutEngine/src/dnpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
		"%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
    }

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "DN_PLATFORM_WINDOWS",
            "DN_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }
    
    filter "configurations:Debug"
        defines "DN_DEBUG"
		runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "DN_RELEASE"
		runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "DN_DIST"
		runtime "Release"
        optimize "On"
    

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
	staticruntime "off"

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
        "DonutEngine/src",
        "%{IncludeDir.glm}"
    }

    links
    {
        "DonutEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "DN_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DN_DEBUG"
		buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "DN_RELEASE"
		buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "DN_DIST"
		buildoptions "/MD"
        optimize "On"