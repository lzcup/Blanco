#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImguiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"

namespace Blanco
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
		void OnEvent(Event& event);
		void Run();
	private:
		static Application* s_Instance;
		Ref<Window> m_Window;
		ImguiLayer* m_Imgui;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_Time = 0.0f;
	private:
		bool OnWindowClose(WindowCloseEvent& event);
	};

	//to be defined in client
	Application* CreatApplication();
}

