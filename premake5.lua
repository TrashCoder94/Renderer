include "./ThirdParty/premake/premake_customization/solution_items.lua"

-- Visual Studio Solution
workspace "Renderer"
	architecture "x86_64"
	startproject "Sandbox"
	
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