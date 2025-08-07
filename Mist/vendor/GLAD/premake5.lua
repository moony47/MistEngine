project "GLAD"
    kind "StaticLib"
    language "C"

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
        staticruntime "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"