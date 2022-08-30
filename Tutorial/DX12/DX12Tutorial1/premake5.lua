project "DX12Tutorial1"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	entrypoint "WinMainCRTStartup"

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/DX12Lib/inc/d3dx12.h",
		"Source/Tutorial1/src/main.cpp"
	}
	
	includedirs
	{
		"Source",
		"%{prj.location}/Source/DX12Lib/inc/",
		"%{prj.location}/Source/externals/boost-1.65.1/"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines	{ "PLATFORM_WINDOWS", "WIN32_LEAN_AND_MEAN" }
		links { "dxgi.lib", "d3d12.lib" }
		
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