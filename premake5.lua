workspace "Blanco"
configurations {"Debug","Release","Dist"}
architecture "x64"

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
    staticruntime "on"
    systemversion "latest"
    
    defines{
    	"BLANCO_BUILD_DLL",
    	"BLANCO_PLATFORM_WINDOWS"
    }
    
    postbuildcommands { "{COPYDIR} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/Sandbox/" }
  
  filter "configurations:Debug"
    defines {
        "BL_DEBUG",
        "BL_ASSERT_ENABLE"
    }
    symbols "On"
    buildoptions "/MDd"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "On"
    buildoptions "/MD"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "On"
    buildoptions "/MD"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"

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
    staticruntime "on"
    systemversion "latest"
    
    defines{
    	"BLANCO_PLATFORM_WINDOWS"
    }
  
  filter "configurations:Debug"
    defines "BL_DEBUG"
    symbols "On"
    buildoptions "/MDd"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "On"
    buildoptions "/MD"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "On"
    buildoptions "/MD"

