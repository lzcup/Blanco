#include "BLpch.h"
#include "OpenGLContext.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace Blanco
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowhandle):m_WindowHandle(windowhandle)
	{
		BL_CORE_ASSERT(m_WindowHandle, "windowhandle is null!")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int version = gladLoadGL(glfwGetProcAddress);
		BL_CORE_ASSERT(version, "Glad initalize failed");
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}

