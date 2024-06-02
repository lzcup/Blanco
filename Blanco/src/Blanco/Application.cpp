#include "BLpch.h"
#include "Application.h"
#include "glad/gl.h"
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
		m_Imgui = new ImguiLayer();
		PushOverLayer(m_Imgui);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 3] = {
			-0.5f,-0.5f,0.0f,
			 0.5f,-0.5f,0.0f,
			 0.0f, 0.5f,0.0f
		};
		m_VertexBuffer.reset(VertexBuffer::CreatVertextBuffer(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		unsigned int indices[3] = {
			0,1,2
		};
		m_IndexBuffer.reset(IndexBuffer::CreatIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
           #version 330
           layout(location = 0) in vec3 a_Position;

           out vec3 o_Position;

           void main(){
                 gl_Position=vec4(a_Position,1.0f);
                 o_Position=a_Position;
           }
        )";

		std::string fragmentSrc = R"(
           #version 330
           layout(location = 0) out vec4 color;

           in vec3 o_Position;

           void main(){
                 color=vec4(o_Position*0.5+0.5,1.0f);
           }
        )";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		m_Shader->Bind();
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
		    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		    glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end();)
				(*it++)->OnUpdate();
			m_Imgui->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImguiRender();
			m_Imgui->End();
			
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}

