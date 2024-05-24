#include "BLpch.h"
#include "WindowsWindow.h"

namespace Blanco {
	static bool s_Initialized = false;

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
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	inline unsigned int WindowsWindow::GetWidth() const
	{
		return m_Data.m_Width;
	}
	inline unsigned int WindowsWindow::GetHeight() const
	{
		return m_Data.m_Height;
	}

	void WindowsWindow::SetEventCallBack(EventCallBackFnc& fnc)
	{
		m_Data.m_Fnc = fnc;
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

		if (!s_Initialized) {
			int success = glfwInit();
			BL_CORE_ASSERT(success, "cannot create window")

			s_Initialized = true;
		}
		m_Window = glfwCreateWindow((int)m_Data.m_Width, (int)m_Data.m_Height, m_Data.m_Title.c_str(), nullptr, nullptr);

		
		BL_CORE_ASSERT(m_Window,"Cannot create a window!")
		BL_CORE_INFO("Create a window({0},{1},{2})", m_Data.m_Title, m_Data.m_Width, m_Data.m_Height);
		
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		m_Data.m_VSync = true;
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
	}

	Window* Window::Create(const WindowProp& prop) {
		return new WindowsWindow(prop);
	}
}
