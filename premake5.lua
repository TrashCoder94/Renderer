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
IncludeDir["GLFW"] = "%{wks.location}/Renderer/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Renderer/ThirdParty/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Renderer/ThirdParty/glm"
IncludeDir["stb_image"] = "%{wks.location}/Renderer/ThirdParty/stb_image"

-- Other premakes to use
group "Dependencies"
	include "ThirdParty/premake"
	include "Renderer/ThirdParty/GLFW"
	include "Renderer/ThirdParty/Glad"
group ""

group "RendererLibrary"
	include "BuildMachineTest"
	include "Renderer"
	include "Sandbox"
group ""