workspace "Blanco"
configurations {"Debug","Release","Dist"}
architecture "x64"

project "Blanco"
  location "Blanco"
  kind "SharedLib"
  language "C++"
  
  outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")
  files {
  	"%{prj.name}/src/**.h",
  	"%{prj.name}/src/**.cpp"
  }
  
  includedirs{
  	"%{prj.name}/vendor/spdlog/include",
    "%{prj.name}/src"
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
    defines "BL_DEBUG"
    symbols "On"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "On"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "On"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"

  links {"Blanco"}
  
  outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")
  files {
  	"%{prj.name}/src/**.h",
  	"%{prj.name}/src/**.cpp"
  }
  
  includedirs{
  	"Blanco/vendor/spdlog/include",
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
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "On"
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "On"

