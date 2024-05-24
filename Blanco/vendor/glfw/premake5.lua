project "GLFW"
  kind "StaticLib"
  language "C"

  targetdir ("bin/"..outputdir.."/%{prj.name}")
  objdir ("bin-int/"..outputdir.."/%{prj.name}")

  files {
  "include/GLFW/glfw3.h",
  "include/GLFW/glfw3native.h",
  "src/init.c",
  "src/input.c",
  "src/vulkan.c",
  "src/window.c",
  "src/context.c",
  "src/monitor.c",
  "src/monitor.c"
  }

  filter "system:windows"
    buildoptions {"-std=c20","-lgdi32"}
    systemversion "latest"
    staticruntime "on"

    files{
        "src/win32_init.c",
        "src/win32_joystick.c",
        "src/win32_module.c",
        "src/win32_monitor.c",
        "src/win32_thread.c",
        "src/win32_time.c",
        "src/win32_window.c",
        "src/egl_context.c",
        "src/wgl_context.c",
        "src/osmesa_context.c"
    }

    defines{
        "_GLFW_WIN32",
        "_CRT_SECURE_NO_WARNINGS"
    }
  filter {"system:windows","configurations:Release"}
    buildoptions "/MT"