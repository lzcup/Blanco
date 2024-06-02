#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImguiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

namespace Blanco
{
	class BL_API Application
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
		std::unique_ptr<Window> m_Window;
		ImguiLayer* m_Imgui;
		bool m_Running = true;
		LayerStack m_LayerStack;
		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		bool OnWindowClose(WindowCloseEvent& event);
	};

	//to be defined in client
	Application* CreatApplication();
}

