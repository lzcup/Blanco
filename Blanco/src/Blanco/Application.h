#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace Blanco
{
	class BL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
		void OnEvent(Event& event);
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(Event& event);
	};

	//to be defined in client
	Application* CreatApplication();
}

