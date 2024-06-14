#include "BLpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include "Blanco/Events/ApplicationEvent.h"
#include "Blanco/Renderer/Renderer.h"
#include "Blanco/Core/TimeStep.h"

namespace Blanco
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BL_CORE_ASSERT(!s_Instance, "Instance already exist!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallBack(BL_BIND_EVENT_FNC(Application::OnEvent));

		Renderer::Init();

		m_Imgui = new ImguiLayer();
		PushOverLayer(m_Imgui);
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

	void Application::OnEvent(Event& e)
	{
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BL_BIND_EVENT_FNC(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BL_BIND_EVENT_FNC(Application::OnWindowResize));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.GetHandled())
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float currentTime = (float)glfwGetTime();
			TimeStep ts = currentTime - m_Time;
			m_Time = currentTime;

			m_Window->Update();

			if (!m_Minimized) {
				for (auto it = m_LayerStack.begin(); it != m_LayerStack.end();)
					(*it++)->OnUpdate(ts);
			}
			m_Imgui->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImguiRender();
			m_Imgui->End();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		Renderer::OnWindowResize(0, 0, (float)e.GetWidth(), (float)e.GetHeight());
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
		}
		else {
			m_Minimized = false;
		}
		return false;
	}
}

