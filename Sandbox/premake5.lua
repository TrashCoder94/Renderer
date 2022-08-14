project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	links
	{
		"Renderer"
	}

	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}
	
	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Renderer}"
	}
	
	libdirs
	{
		"%{wks.location}/Binaries/" .. outputdir .. "/Renderer"
	}
	
	filter "system:windows"
		systemversion "latest"
	
	filter "system:macosx"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		filter "system:windows"
			links{ "Renderer_Debug.lib" }
		filter "system:linux"
			links{ "libRenderer_Debug.a" }
			
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
		filter "system:windows"
			links{ "Renderer_Release.lib" }
		filter "system:linux"
			links{ "libRenderer_Release.a" }
		
	filter "configurations:Distribution"
		defines "DISTRIBUTION"
		runtime "Release"
		optimize "on"
		filter "system:windows"
			links{ "Renderer_Distribution.lib" }
		filter "system:linux"
			links{ "libRenderer_Distribution.a" }