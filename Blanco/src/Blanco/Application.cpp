#include "Application.h"
#include "Events/Mouse.h"
#include "Log.h"

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

