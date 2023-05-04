workspace "VoxelEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/VoxelEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/VoxelEngine/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/VoxelEngine/vendor/glm"

group "Dependencies"
	include "VoxelEngine/vendor/GLFW"
	include "VoxelEngine/vendor/Glad"
group ""

include "VoxelEngine/vendor/GLFW"
include "VoxelEngine/vendor/Glad"

project "VoxelEngine"
	location "VoxelEngine"
	kind	 "StaticLib"
	language "c++"
	cppdialect "c++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}
	
	includedirs
	{
		"VoxelEngine/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "10.0"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE",
			"ENGINE_ENABLE_ASSERTS"
		}

		filter "configurations:Debug"
			defines	 "ENGINE_DEBUG"
			buildoptions "/MDd"
			symbols "on"

		filter "configurations:Debug-Editor"
			defines
			{
				"ENGINE_DEBUG",
				"ENGINE_EDITOR"
			}
			buildoptions "/MDd"
			symbols "on"

		filter "configurations:Release"
			defines	 "ENGINE_RELEASE"
			buildoptions "/MD"
			optimize "on"

		filter "configurations:Dist"
			defines	 "ENGINE_DIST"
			buildoptions "/MD"
			optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "c++"
	cppdialect "c++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir	  ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"VoxelEngine/vendor/spdlog/include",
		"VoxelEngine/vendor",
		"VoxelEngine/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
	}

	links
	{
		"VoxelEngine"
	}

	filter "system:windows"
		systemversion "10.0"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines	 "ENGINE_DEBUG"
			buildoptions "/MDd"
			symbols "on"

		filter "configurations:Debug-Editor"
			defines
			{
				"ENGINE_DEBUG",
				"ENGINE_EDITOR"
			}
			buildoptions "/MDd"
			symbols "on"

		filter "configurations:Release"
			defines	 "ENGINE_RELEASE"
			buildoptions "/MD"
			optimize "on"

		filter "configurations:Dist"
			defines	 "ENGINE_DIST"
			buildoptions "/MD"
			optimize "on"