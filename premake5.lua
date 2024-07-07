workspace "Blanco"
configurations {"Debug","Release","Dist"}
architecture "x64"
cppdialect "C++20"
startproject "BlancoPelota"

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include directories
IncludeDir = {}
IncludeDir["spdlog"] = "Blanco/vendor/spdlog/include"
IncludeDir["GLFW"] = "Blanco/vendor/glfw/include"
IncludeDir["Glad"] = "Blanco/vendor/glad/include"
IncludeDir["ImGui"] = "Blanco/vendor/imgui"
IncludeDir["glm"] = "Blanco/vendor/glm/glm"
IncludeDir["stb_image"] = "Blanco/vendor/stb_image"
IncludeDir["entt"] = "Blanco/vendor/entt/include"
IncludeDir["Yaml"] = "Blanco/vendor/yaml/include"

include "Blanco/vendor/glfw"
include "Blanco/vendor/glad"
include "Blanco/vendor/imgui"
include "Blanco/vendor/yaml"

project "Blanco"
  location "Blanco"
  kind "StaticLib"
  language "C++"
  staticruntime "on"

  links {
      "GLFW",
      "opengl32.lib",
      "Glad",
      "ImGui",
      "Yaml"
  }

  pchheader "BLpch.h"
  pchsource "%{prj.name}/src/BLpch.cpp"
  
  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")
  files {
  	"%{prj.name}/src/**.h",
  	"%{prj.name}/src/**.cpp",
  	"%{prj.name}/vendor/glm/glm/**.inl",
  	"%{prj.name}/vendor/glm/glm/**.hpp",
  	"%{prj.name}/vendor/stb_image/**.h",
  	"%{prj.name}/vendor/stb_image/**.cpp"
  }
  
  includedirs{
    "%{prj.name}/src",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.stb_image}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.Yaml}"
  }

  defines{
    "_CRT_SECURE_NO_WARNINGS",
    "YAML_CPP_STATIC_DEFINE"
  }

  filter "system:windows"
    systemversion "latest"
    
    defines{
    	"BLANCO_BUILD_DLL",
    	"BLANCO_PLATFORM_WINDOWS"
    }
  
  filter "configurations:Debug"
    defines "BL_DEBUG"
    symbols "on"
    runtime "Debug"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "on"
    runtime "Release"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "on"
    runtime "Release"

project "BlancoPelota"
  location "BlancoPelota"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  links {
      "Blanco"
  }
  
  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")
  files {
  	"%{prj.name}/src/**.h",
  	"%{prj.name}/src/**.cpp"
  }
  
  includedirs{
  	"%{IncludeDir.spdlog}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.entt}",
    "Blanco/src"
  }

  filter "system:windows"
    systemversion "latest"
    
    defines{
    	"BLANCO_PLATFORM_WINDOWS"
    }
  
  filter "configurations:Debug"
    defines "BL_DEBUG"
    symbols "on"
    runtime "Debug"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "on"
    runtime "Release"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "on"
    runtime "Release"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  links {
      "Blanco"
  }
  
  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")
  files {
  	"%{prj.name}/src/**.h",
  	"%{prj.name}/src/**.cpp"
  }
  
  includedirs{
  	"%{IncludeDir.spdlog}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.ImGui}",
    "Blanco/src"
  }

  filter "system:windows"
    systemversion "latest"
    
    defines{
    	"BLANCO_PLATFORM_WINDOWS"
    }
  
  filter "configurations:Debug"
    defines "BL_DEBUG"
    symbols "on"
    runtime "Debug"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "on"
    runtime "Release"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "on"
    runtime "Release"

