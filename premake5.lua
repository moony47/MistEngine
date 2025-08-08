workspace "MistEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Mist/vendor/GLFW/include"
IncludeDir["GLAD"] = "Mist/vendor/GLAD/include"
IncludeDir["ImGui"] = "Mist/vendor/ImGui"

IncludeDir["stb_image"] = "Mist/vendor/stb_image"
IncludeDir["glm"] = "Mist/vendor/glm"

include "Mist/vendor/GLFW"
include "Mist/vendor/GLAD"
include "Mist/vendor/ImGui"

project "Mist"
    location "Mist"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mistpch.h"
    pchsource "Mist/src/mistpch.cpp"

    files
    {
        "premake5.lua",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/src/Mist",
        "%{prj.name}/src/Mist/Tests",
        "%{prj.name}/src/Platform",
        "%{prj.name}/src/Platform/Windows",
        "%{prj.name}/src/Platform/OpenGL",
        "%{prj.name}/src/vendor",
        "%{prj.name}/vendor",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
    }

    links 
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32"
    }

    filter "system:windows"
        cppdialect "C++23"
        systemversion "latest"

        defines
        {
            "MIST_PLATFORM_WINDOWS",
            "MIST_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/%{cfg.buildtarget.name}")
        }

    filter "configurations:Debug"
        defines 
        {
            "MIST_DEBUG",
            "MIST_ENABLE_ASSERTS"
        }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "MIST_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "MIST_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Mist/src",
        "Mist/vendor/spdlog/include"
    }

    links
    {
        "Mist"
    }

    filter "system:windows"
        cppdialect "C++23"
        systemversion "latest"

        defines
        {
            "MIST_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "MIST_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "MIST_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "MIST_DIST"
        runtime "Release"
        optimize "On"