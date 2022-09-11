project "DX12Tutorial4"
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
		"Source/Tutorial4/shaders/**.hlsl",
		"Source/DX12Lib/Resources/Shaders/**.hlsl"
	}
	
	includedirs
	{
		"Source",
		"%{prj.location}/Source/DirectXTex/DDSTextureLoader/",
		"%{prj.location}/Source/DirectXTex/DirectXTex/",
		"%{prj.location}/Source/DirectXTex/WICTextureLoader/",
		"%{prj.location}/Source/DX12Lib/inc/",
		"%{prj.location}/Source/Tutorial4/inc/"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	prebuildmessage "Copying Assets to binaries folder!"
	prebuildcommands
	{
		"{COPY} %{prj.location}Source/externals %{cfg.targetdir}",
		"{COPY} %{prj.location}Source/Assets %{cfg.targetdir}/Assets/"
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
	filter { "files:**-fx.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Effect"
	filter { "files:**-vs.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Vertex"
	filter { "files:**-ps.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Pixel"
	filter { "files:**-gs.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Geometry"
	filter { "files:**-hs.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Hull"
	filter { "files:**-ds.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Domain"
	filter { "files:**-cs.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Compute"
	filter { "files:**-ms.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Mesh"
	filter { "files:**-as.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Amplification"
	filter {}