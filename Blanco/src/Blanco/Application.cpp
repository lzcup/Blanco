#include "BLpch.h"
#include "Application.h"
#include "Events/Mouse.h"
#include "GLFW/glfw3.h"

namespace Blanco
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Window->Update();
		while (m_Running)
		{}
	}
}

