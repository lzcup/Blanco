project "Blanco"
  kind "StaticLib"
  language "C++"
  staticruntime "on"

  links {
      "GLFW",
      "opengl32.lib",
      "Glad",
      "ImGui",
      "Yaml",
      "Box2D"
  }

  pchheader "BLpch.h"
  pchsource "src/BLpch.cpp"
  
  targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
  objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

  files {
  	"src/**.h",
  	"src/**.cpp",
  	"vendor/glm/glm/**.inl",
  	"vendor/glm/glm/**.hpp",
  	"vendor/stb_image/**.h",
  	"vendor/stb_image/**.cpp",
  	"vendor/ImGuizmo/ImGuizmo.h",
  	"vendor/ImGuizmo/ImGuizmo.cpp",
  }
  
  includedirs{
    "src",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.stb_image}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.Yaml}",
    "%{IncludeDir.ImGuizmo}",
    "%{IncludeDir.Box2D}"
  }

  defines{
    "_CRT_SECURE_NO_WARNINGS",
    "YAML_CPP_STATIC_DEFINE"
  }

  filter "files:vendor/ImGuizmo/**.cpp"
    flags{
        "NoPCH"
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

    links
	{
	    "%{Library.ShaderC_Debug}",
	    "%{Library.SPIRV_Cross_Debug}",
	    "%{Library.SPIRV_Cross_GLSL_Debug}"
	}
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "on"
    runtime "Release"

    links
	{
		"%{Library.ShaderC_Release}",
		"%{Library.SPIRV_Cross_Release}",
		"%{Library.SPIRV_Cross_GLSL_Release}"
	}
  
  filter "configurations:Dist"
    defines "BL_DIST"
    optimize "on"
    runtime "Release"

    links
	{
		"%{Library.ShaderC_Release}",
		"%{Library.SPIRV_Cross_Release}",
		"%{Library.SPIRV_Cross_GLSL_Release}"
	}