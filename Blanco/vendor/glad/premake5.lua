project "Glad"
  kind "StaticLib"
  language "C"

  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")

  files {
      "include/glad/gl.h",
      "include/KHR/khrplatform.h",
      "src/gl.c"
  }

  includedirs{
      "include"
  }

  filter "system:windows"
    systemversion "latest"
    staticruntime "on"

  filter "configurations:Debug"
    defines "BL_DEBUG"
    symbols "on"
    runtime "Debug"
  
  filter "configurations:Release"
    defines "BL_RELEASE"
    optimize "on"
    runtime "Release"
