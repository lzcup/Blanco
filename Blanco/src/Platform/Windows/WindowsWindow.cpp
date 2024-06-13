#include "BLpch.h"
#include "WindowsWindow.h"
#include "Blanco/Events/ApplicationEvent.h"
#include "Blanco/Events/KeyEvent.h"
#include "Blanco/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Blanco {
	static bool s_Initialized = false;

	static void ErrorCallBack(int error, const char* description)
	{
		BL_CORE_ERROR("GLFW error({0}):{1}",error,description);
	}

	WindowsWindow::WindowsWindow(const WindowProp& prop)
	{
		Init(prop);
	}
	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}
	void WindowsWindow::Update()
	{
		m_Context->SwapBuffer();
		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.m_VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.m_VSync;
	}

	void WindowsWindow::Init(const WindowProp& prop)
	{
		m_Data.m_Width = prop.m_Width;
		m_Data.m_Height = prop.m_Height;
		m_Data.m_Title = prop.m_Title;

		glfwSetErrorCallback(ErrorCallBack);

		if (!s_Initialized) {
			int success = glfwInit();
			BL_CORE_ASSERT(success, "cannot create window")

			s_Initialized = true;
		}
		m_Window = glfwCreateWindow((int)m_Data.m_Width, (int)m_Data.m_Height, m_Data.m_Title.c_str(), nullptr, nullptr);
		
		BL_CORE_ASSERT(m_Window,"Cannot create a window!")
		BL_CORE_INFO("Create a window({0},{1},{2})", m_Data.m_Title, m_Data.m_Width, m_Data.m_Height);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//set callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.m_Fnc(event);
			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_Width = width;
			data.m_Height = height;

			WindowResizeEvent event(width, height);
			data.m_Fnc(event);
			});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowMovedEvent event((float)xpos, (float)ypos);
			data.m_Fnc(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			  case GLFW_RELEASE:
			  {
			  	KeyReleasedEvent event(key, scancode);
			  	data.m_Fnc(event);
			  	break;
			  }
			  case GLFW_PRESS:
			  {
			  	KeyPressedEvent event(key, scancode, 0);
			  	data.m_Fnc(event);
			  	break;
			  }
			  case GLFW_REPEAT: {
			  	KeyPressedEvent event(key, scancode, 1);
			  	data.m_Fnc(event);
			  	break;
			  }
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(codepoint);
			data.m_Fnc(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.m_Fnc(event);
				break;
			}
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.m_Fnc(event);
				break;
			}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.m_Fnc(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xpos, (float)ypos);
			data.m_Fnc(event);
			});
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
	}

	Window* Window::Create(const WindowProp& prop) {
		return new WindowsWindow(prop);
	}
}
