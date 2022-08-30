project "DX12Tutorial2"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	entrypoint "WinMainCRTStartup"

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/Tutorial2/shaders/**.hlsl"
	}
	
	includedirs
	{
		"Source",
		"%{prj.location}/Source/DX12Lib/inc/",
		"%{prj.location}/Source/Tutorial2/inc/"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	prebuildmessage "Copying Assets to binaries folder!"
	prebuildcommands
	{
		"{COPY} %{prj.location}Source/externals %{cfg.targetdir}"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines	{ "PLATFORM_WINDOWS", "WIN32_LEAN_AND_MEAN" }
		links { "dxguid.lib", "dxgi.lib", "d3d12.lib", "d3dcompiler.lib", "shlwapi.lib" }
		
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
		
	filter { "files:**.hlsl" }
		flags "ExcludeFromBuild"
		shadermodel "6.0"
	filter { "files:**-p.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Pixel"
	filter { "files:**-v.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Vertex"
	filter {}