#pragma once
#include "Window.h"
#include "LayerStack.h"
#include "Blanco/Events/ApplicationEvent.h"
#include "Blanco/ImGui/ImguiLayer.h"
#include "Blanco/Renderer/Shader.h"
#include "Blanco/Renderer/Buffer.h"
#include "Blanco/Renderer/VertexArray.h"
#include "Blanco/Renderer/Camera.h"

int main(int argc, char** argv);

namespace Blanco
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			BL_CORE_ASSERT(index < Count,"");
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "Blanco App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		ImguiLayer& GetImgui() { return *m_Imgui; }

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
		void OnEvent(Event& e);
		void Close();
		
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Ref<Window> m_Window;
		ImguiLayer* m_Imgui;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_Time = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	//to be defined in client
	Application* CreatApplication(ApplicationCommandLineArgs args);
}

