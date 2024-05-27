#include "BLpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include "Events/ApplicationEvent.h"

namespace Blanco
{


	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BL_CORE_ASSERT(!s_Instance, "Instance already exist!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BL_BIND_EVENT_FNC(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverLayer(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		Dispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BL_BIND_EVENT_FNC(Application::OnWindowClose));
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
			m_Window->Update();
		    glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
		    glClear(GL_COLOR_BUFFER_BIT);
			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end();)
				(*it++)->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}

