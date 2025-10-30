project "GLAD"
    kind "StaticLib"
    language "C"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "premake5.lua",

		"include/GLAD/glad.h",
		"include/KHR/khrplatform.h",

		"src/glad.c",
    }

	includedirs 
	{
		"include"
	}

    filter "system:windows"
        systemversion "latest"
            
        buildoptions {
            "/MP"
        }

    filter "configurations:Debug"
        runtime "Debug"
		symbols "On"

    filter "configurations:Release"
        runtime "Release"
		optimize "On"