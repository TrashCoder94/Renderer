include "./ThirdParty/premake/premake_customization/solution_items.lua"

-- Visual Studio Solution
workspace "Renderer"
	architecture "x86_64"
	startproject "DX12Tutorial2"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
	linkoptions 
	{ 
		"-IGNORE:4006"
	}
	
-- Folder to put Binaries and Intermediate files into
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories
IncludeDir = {}
IncludeDir["Renderer"] = "%{wks.location}/Renderer/Source/"

-- Other premakes to use
group "Dependencies"
	include "ThirdParty/premake"
group ""

group "RendererLibrary"
	include "BuildMachineTest"
	include "Renderer"
	include "Sandbox"
group ""

group "Tutorials"
	include "Tutorial/DX12/DX12Tutorial1"
	include "Tutorial/DX12/DX12Tutorial2"
	include "Tutorial/DX12/DX12Tutorial3"
group ""