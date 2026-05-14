workspace "MistEngine"
    architecture "x64"
    startproject "Mistwraith"
    
    platforms { "x64" }
    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["glm"] = "Mist/vendor/glm"

IncludeDir["GLFW"] = "Mist/vendor/GLFW/include"
IncludeDir["GLAD"] = "Mist/vendor/GLAD/include"

IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

IncludeDir["shaderc"] = "Mist/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "Mist/vendor/SPIRV-Cross"

IncludeDir["ImGui"] = "Mist/vendor/ImGui"
IncludeDir["ImGuizmo"] = "Mist/vendor/ImGuizmo"

IncludeDir["stb_image"] = "Mist/vendor/stb_image"
IncludeDir["entt"] = "Mist/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Mist/vendor/yaml-cpp/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

group "Dependencies"
    include "Mist/vendor/GLFW"
    include "Mist/vendor/GLAD"
    include "Mist/vendor/ImGui"
    include "Mist/vendor/yaml-cpp"

group ""
    project "ScriptEngine"
        location "ScriptEngine"
        architecture "x64"
        kind "SharedLib"
        language "C#"
        dotnetframework "net10.0"
        clr "Unsafe"
        
        files {
            "%{prj.name}/**.cs",
        }

        filter "platforms:AnyCPU"
            vsprops {
		        GenerateRuntimeConfigurationFiles = "true"
	        }

    project "Mist"
        location "Mist"
        kind "StaticLib"
        language "C++"
        cppdialect "C++23"
        staticruntime "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "mistpch.h"
        pchsource "Mist/src/mistpch.cpp"

        files {
            "premake5.lua",
            "%{prj.name}/.clang-format",

            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",

		    "%{prj.name}/vendor/stb_image/**.h",
		    "%{prj.name}/vendor/stb_image/**.cpp",

		    "%{prj.name}/vendor/glm/glm/**.hpp",
		    "%{prj.name}/vendor/glm/glm/**.inl",

		    "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		    "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
        }


        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "GLFW_INCLUDE_NONE",
            "YAML_CPP_STATIC_DEFINE"
        }

        includedirs {
            "%{prj.name}/src",
            "%{prj.name}/src/Platform",

            "%{prj.name}/vendor",
            "%{prj.name}/vendor/spdlog/include",

            "%{IncludeDir.GLFW}",
            "%{IncludeDir.GLAD}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.yaml_cpp}",
            "%{IncludeDir.VulkanSDK}",
        }

        links {
            "GLFW",
            "GLAD",
            "ImGui",
            "yaml-cpp",
            "opengl32"
        }

        filter "files:Mist/vendor/ImGuizmo/**.cpp"
            enablepch "on"

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
            symbols "on"
            links {
                "%{Library.ShaderC_Debug}",
                "%{Library.SPIRV_Cross_Debug}",
                "%{Library.SPIRV_Cross_GLSL_Debug}",
            }

        filter "configurations:Release"
            defines {
                "MIST_RELEASE",
                "MIST_PROFILING"
            }
            runtime "Release"
            optimize "on"
            links {
                "%{Library.ShaderC_Release}",
                "%{Library.SPIRV_Cross_Release}",
                "%{Library.SPIRV_Cross_GLSL_Release}",
            }

        filter "configurations:Dist"
            defines "MIST_DIST"
            runtime "Release"
            optimize "on"
            links {
                "%{Library.ShaderC_Release}",
                "%{Library.SPIRV_Cross_Release}",
                "%{Library.SPIRV_Cross_GLSL_Release}",
            }

        filter {}

    project "Mistwraith"
        location "Mistwraith"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++23"
        staticruntime "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/.clang-format",

            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs {
            "Mist/src",

            "Mist/vendor",
            "Mist/vendor/spdlog/include",

            "%{IncludeDir.ImGui}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.yaml_cpp}",

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
            symbols "on"

        filter "configurations:Release"
            defines {
                "MIST_RELEASE",
                "MIST_PROFILING"
            }
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines {
                "MIST_DIST"
            }
            runtime "Release"
            optimize "on"

        filter {}

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

        filter {}