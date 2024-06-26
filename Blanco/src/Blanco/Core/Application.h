#pragma once
#include "Window.h"
#include "LayerStack.h"
#include "Blanco/Events/ApplicationEvent.h"
#include "Blanco/ImGui/ImguiLayer.h"
#include "Blanco/Renderer/Shader.h"
#include "Blanco/Renderer/Buffer.h"
#include "Blanco/Renderer/VertexArray.h"
#include "Blanco/Renderer/Camera.h"

namespace Blanco
{
	class Application
	{
	public:
		Application(const char* name = "Blanco App");
		virtual ~Application();

		static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		ImguiLayer& GetImgui() { return *m_Imgui; }

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
		void OnEvent(Event& e);
		void Run();
		void Close();
	private:
		static Application* s_Instance;
		Ref<Window> m_Window;
		ImguiLayer* m_Imgui;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_Time = 0.0f;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	};

	//to be defined in client
	Application* CreatApplication();
}

