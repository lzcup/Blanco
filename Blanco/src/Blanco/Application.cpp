#include "BLpch.h"
#include "Application.h"
#include "Events/Mouse.h"

namespace Blanco
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		MouseButtonPressedEvent e(666);
		BL_CORE_TRACE(e.ToString());
		while (true)
		{
		}
	}
}

