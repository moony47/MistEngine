workspace "MistEngine"
    architecture "x64"
    startproject "Mistwraith"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Mist/vendor/GLFW/include"
IncludeDir["GLAD"] = "Mist/vendor/GLAD/include"
IncludeDir["ImGui"] = "Mist/vendor/ImGui"
IncludeDir["glm"] = "Mist/vendor/glm"
IncludeDir["stb_image"] = "Mist/vendor/stb_image"
IncludeDir["entt"] = "Mist/vendor/entt/include"

group "Dependencies"
    include "Mist/vendor/GLFW"
    include "Mist/vendor/GLAD"
    include "Mist/vendor/ImGui"

group ""
    project "Mist"
        location "Mist"
        kind "StaticLib"
        language "C++"
        cppdialect "C++23"
        staticruntime "On"
        

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "mistpch.h"
        pchsource "Mist/src/mistpch.cpp"

        files {
            "premake5.lua",
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
		    "%{prj.name}/vendor/glm/glm/**.hpp",
		    "%{prj.name}/vendor/glm/glm/**.inl",
		    "%{prj.name}/vendor/stb_image/**.h",
		    "%{prj.name}/vendor/stb_image/**.cpp",
            "%{prj.name}/.clang-format"
        }


        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "GLFW_INCLUDE_NONE"
        }

        includedirs {
            "%{prj.name}/src",
            "%{prj.name}/src/Platform",

            "%{prj.name}/vendor",
            "%{prj.name}/vendor/spdlog/include",

            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.entt}",
        }

        links {
            "GLFW",
            "GLAD",
            "ImGui",
            "opengl32"
        }

        filter "system:windows"
            systemversion "latest"

            defines {
                "MIST_PLATFORM_WINDOWS"
            }

            buildoptions {
                "/MP"
            }

        filter "configurations:Debug"
            defines {
                "MIST_DEBUG",
                "MIST_PROFILING"
            }
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines {
                "MIST_RELEASE",
                "MIST_PROFILING"
            }
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            defines "MIST_DIST"
            runtime "Release"
            optimize "On"

    project "Mistwraith"
        location "Mistwraith"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++23"
        staticruntime "On"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/.clang-format"
        }

        includedirs {
            "Mist/src",

            "Mist/vendor",
            "Mist/vendor/spdlog/include",

            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",

        }

        links {
            "Mist"
        }

        filter "system:windows"
            systemversion "latest"

            defines {
                "MIST_PLATFORM_WINDOWS"
            }
            
            buildoptions {
                "/MP"
            }

        filter "configurations:Debug"
            defines {
                "MIST_DEBUG",
                "MIST_PROFILING"
            }
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines {
                "MIST_RELEASE",
                "MIST_PROFILING"
            }
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            defines {
                "MIST_DIST"
            }
            runtime "Release"
            optimize "On"

    project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++23"
        staticruntime "On"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/.clang-format"
        }

        includedirs {
            "Mist/src",

            "Mist/vendor",
            "Mist/vendor/spdlog/include",

            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",
        }

        links {
            "Mist"
        }

        filter "system:windows"
            systemversion "latest"

            defines {
                "MIST_PLATFORM_WINDOWS"
            }

        filter "configurations:Debug"
            defines {
                "MIST_DEBUG",
                "MIST_PROFILING"
            }
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines {
                "MIST_RELEASE",
                "MIST_PROFILING"
            }
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            defines {
                "MIST_DIST"
            }
            runtime "Release"
            optimize "On"