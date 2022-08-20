project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	entrypoint "WinMainCRTStartup"
		
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
		"%{IncludeDir.Renderer}"
	}
	
	libdirs
	{
		"%{wks.location}/Binaries/" .. outputdir .. "/Renderer/"
	}
	
	newoption 
	{
		trigger     = "--width",
		value 		= 800,
		description = "Sets window width"
	}
	
	newoption 
	{
		trigger     = "--height",
		value 		= 600,
		description = "Sets window height"
	}
	
	newoption 
	{
		trigger     = "--warp",
		value 		= false,
		description = "Whether DX12 should use warp for older graphics cards"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines	{ "PLATFORM_WINDOWS" }
		
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