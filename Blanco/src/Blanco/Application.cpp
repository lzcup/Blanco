#include "BLpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include "Events/ApplicationEvent.h"

namespace Blanco
{
#define BL_BIND_FNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BL_BIND_FNC(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverLayer(layer);
	}

	void Application::OnEvent(Event& event)
	{
		BL_CORE_TRACE(event.ToString());
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BL_BIND_FNC(OnWindowClose));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.GetHandled())
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->Update();
			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end();)
				(*it++)->OnUpdate();
		}
	}

	bool Application::OnWindowClose(Event& event)
	{
		m_Running = false;
		return true;
	}
}

