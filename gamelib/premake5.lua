project "gamelib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/obj/" .. outputdir .. "/%{prj.name}")

	pchheader "bqpch.h"
	pchsource "source/bqpch.cpp"
	
	files
	{
		"include/**.h",
		"source/**.cpp",
    }
	includedirs
	{
		"include",
		"%{IncludeDir.SFML}",
		"vendor/include",
	}
	filter "system:windows"
		systemversion "latest"
		defines{ "PLATFORM_WINDOWS" }
		local vendor_root = "../vendor"
		local vendor_includes = vendor_root  .. "/include"
		includedirs 
		{ 
			vendor_includes 
		}
		
	filter "system:linux"
		systemversion "latest"
		defines{ "PLATFORM_LINUX" }
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"