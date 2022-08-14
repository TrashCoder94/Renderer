project "Renderer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	-- Binaries and Intermediate folders
	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	-- Which files to include in our Visual Studio project
	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"ThirdParty/stb_image/**.h",
		"ThirdParty/stb_image/**.cpp",
		"ThirdParty/glm/glm/**.hpp",
		"ThirdParty/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	-- Additional include directories
	includedirs
	{
		"Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}
	
	-- Linking other projects and libraries
	links
	{
		"GLFW",
		"Glad"
	}

	-- Additional library directories
	libdirs
	{
		"ThirdParty/GLFW/Binaries/" .. outputdir .. "/GLFW",
		"ThirdParty/Glad/Binaries/" .. outputdir .. "/Glad"
	}

	-- Configuration setup for Windows
	filter "system:windows"
		systemversion "latest"
		defines	{ "PLATFORM_WINDOWS" }
		links { "opengl32.lib" }
		
	filter "system:macosx"
		systemversion "latest"
		defines { "PLATFORM_MAC" }
		
	filter "system:linux"
		systemversion "latest"
		defines { "PLATFORM_LINUX" }
		links { "glfw3", "GL" }
		postbuildcommands { "ls -R %{wks.location}/Binaries/", "ls -R %{wks.location}/Renderer/ThirdParty/GLFW/Binaries/", "ls -R %{wks.location}/Renderer/ThirdParty/Glad/Binaries/" }
		
	-- Debug configuration, turn debug symbols on
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		
	-- Release configuration, optimizations turned on
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
		
	-- Distribution configuration, optimizations turned on
	filter "configurations:Distribution"
		defines "DISTRIBUTION"
		runtime "Release"
		optimize "on"