#include "BLpch.h"
#include "Application.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "Events/ApplicationEvent.h"

namespace Blanco
{


	static GLenum ShaderDataTypeToGLType(ShaderDataType type) {
		switch (type) {
		    case ShaderDataType::Float:    return GL_FLOAT;
		    case ShaderDataType::Float2:   return GL_FLOAT;
		    case ShaderDataType::Float3:   return GL_FLOAT;
		    case ShaderDataType::Float4:   return GL_FLOAT;
		    case ShaderDataType::Mat3:     return GL_FLOAT;
		    case ShaderDataType::Mat4:     return GL_FLOAT;
		    case ShaderDataType::Int:      return GL_INT;
		    case ShaderDataType::Int2:     return GL_INT;
		    case ShaderDataType::Int3:     return GL_INT;
		    case ShaderDataType::Int4:     return GL_INT;
		    case ShaderDataType::Bool:     return GL_BOOL;
		}
		BL_CORE_ASSERT(false, "Unknown ShaderDataType!")
	    return 0;
	}

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

		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f,0.2f,0.4f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f,0.5f,0.7f,0.5f,1.0f,
			 0.0f, 0.5f,0.0f,0.8f,0.2f,0.3f,1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::CreatVertextBuffer(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
			    {ShaderDataType::Float3,"a_Position",false},
				{ShaderDataType::Float4,"a_Color",false}
			};
			m_VertexBuffer->SetLayout(layout);
		}

		uint8_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout()) {
			glEnableVertexAttribArray(index);
		    glVertexAttribPointer(index, element.GetComponentSize(),
		    	ShaderDataTypeToGLType(element.Type), 
		    	element.Normalized, 
		    	m_VertexBuffer->GetLayout().GetStride(), 
		    	(const void*)element.Offset);
		    index++;
		}

		unsigned int indices[3] = {
			0,1,2
		};
		m_IndexBuffer.reset(IndexBuffer::CreatIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
           #version 330
           layout(location = 0) in vec3 a_Position;
           layout(location = 1) in vec4 a_Color;

           out vec3 o_Position;
           out vec4 o_Color;

           void main(){
                 gl_Position=vec4(a_Position,1.0f);
                 o_Position=a_Position;
                 o_Color=a_Color;
           }
        )";

		std::string fragmentSrc = R"(
           #version 330
           layout(location = 0) out vec4 color;

           in vec3 o_Position;
           in vec4 o_Color;

           void main(){
                 color=vec4(o_Position*0.5+0.5,1.0f);
                 color=o_Color;
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

