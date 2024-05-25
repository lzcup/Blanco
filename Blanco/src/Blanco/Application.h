#pragma once
#include "Core.h"
#include "Window.h"

namespace Blanco
{
	class BL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& event);
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		bool OnWindowClose(Event& event);
	};

	//to be defined in client
	Application* CreatApplication();
}

