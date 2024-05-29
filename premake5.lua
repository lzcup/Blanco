workspace "Blanco"
configurations {"Debug","Release","Dist"}
architecture "x64"
startproject "Sandbox"

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include directories
IncludeDir = {}
IncludeDir["spdlog"] = "Blanco/vendor/spdlog/include"
IncludeDir["GLFW"] = "Blanco/vendor/glfw/include"
IncludeDir["Glad"] = "Blanco/vendor/glad/include"
IncludeDir["ImGui"] = "Blanco/vendor/imgui"

include "Blanco/vendor/glfw"
include "Blanco/vendor/glad"
include "Blanco/vendor/imgui"

project "Blanco"
  location "Blanco"
  kind "SharedLib"
  language "C++"
  staticruntime "off"

  links {
      "GLFW",
      "opengl32.lib",
      "Glad",
      "ImGui"
  }

  pchheader "BLpch.h"
  pchsource "%{prj.name}/src/BLpch.cpp"
  
  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")
  files {
  	"%{prj.name}/src/**.h",
  	"%{prj.name}/src/**.cpp"
  }
  
  includedirs{
    "%{prj.name}/src",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}"

  }

  filter "system:windows"
    cppdialect "C++20"
    systemversion "latest"
    
    defines{
    	"BLANCO_BUILD_DLL",
    	"BLANCO_PLATFORM_WINDOWS"
    }
    
    postbuildcommands { "{COPYDIR} %{cfg.buildtarget.relpath} \"../bin/"..outputdir.."/Sandbox/\"" }
  
  filter "configurations:Debug"
    defines "BL_DEBUG"
    symbols "On"
    runtime "Debug"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "On"
    runtime "Release"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "On"
    runtime "Release"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"
  staticruntime "off"

  links {"Blanco"}
  
  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")
  files {
  	"%{prj.name}/src/**.h",
  	"%{prj.name}/src/**.cpp"
  }
  
  includedirs{
  	"%{IncludeDir.spdlog}",
    "Blanco/src"
  }

  filter "system:windows"
    cppdialect "C++20"
    systemversion "latest"
    
    defines{
    	"BLANCO_PLATFORM_WINDOWS"
    }
  
  filter "configurations:Debug"
    defines "BL_DEBUG"
    symbols "On"
    runtime "Debug"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "On"
    runtime "Release"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "On"
    runtime "Release"

