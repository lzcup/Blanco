project "BlancoPelota"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  links {
      "Blanco"
  }
  
  targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
  objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

  files {
  	"src/**.h",
  	"src/**.cpp"
  }
  
  includedirs{
  	"%{IncludeDir.spdlog}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.ImGuizmo}",
    "%{wks.location}/Blanco/src"
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