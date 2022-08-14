project "BuildMachineTest"
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
		"%{wks.location}/Binaries/" .. outputdir .. "/Renderer/"
	}
	
	postbuildmessage "Copying OpenGL dll for Build Machine!"
	postbuildcommands
	{
		"{COPY} %{wks.location}Renderer/ThirdParty/OpenGL %{cfg.targetdir}"
	}
	
	filter "system:windows"
		systemversion "latest"
		links{ "Renderer.lib" }
		
	filter "system:macosx"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"
		links{ "libRenderer.a" }
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
			
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Distribution"
		defines "DISTRIBUTION"
		runtime "Release"
		optimize "on"