project "chess_engine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"source/**.h",
		"source/**.hpp",
		"source/**.cpp",
		"source/surge/**.h",
		"source/surge/**.hpp",
		"source/surge/**.cpp"
	}
	links
	{
		"gamelib"
	}
	includedirs
	{
		"../../gamelib/include",
		"../../gamelib/vendor/include"
	}
	filter "system:windows"
		systemversion "latest"
		defines{ "PLATFORM_WINDOWS" }
		local vendor_root = "../../vendor"
		local vendor_includes = vendor_root  .. "/include"
		includedirs 
		{ 
			vendor_includes 
		}
		filter "configurations:Debug"
			local vendor_libs = vendor_root .. "/lib"
			libdirs 
			{ 
				vendor_libs 
			}
			links
			{
				"sfml-graphics-d",
				"sfml-window-d",
				"sfml-system-d",
				"sfml-audio-d",
				"sfml-network-d",
				"libssl",
				"libcrypto"
			}
		filter "configurations:Release"
			local vendor_libs = vendor_root .. "/lib"
			libdirs 
			{ 
				vendor_libs 
			}
			
			links
			{
				"sfml-graphics",
				"sfml-window",
				"sfml-system",
				"sfml-audio",
				"sfml-network",
				"libssl",
				"libcrypto"
			}
	filter "system:linux"
		systemversion "latest"
		defines{ "PLATFORM_LINUX" }
		links
		{
			"openssl-1.1/ssl",
			"openssl-1.1/crypto"
		}
	
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"