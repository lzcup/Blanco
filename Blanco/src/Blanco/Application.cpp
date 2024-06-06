#include "BLpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Renderer.h"

namespace Blanco
{
	Application* Application::s_Instance = nullptr;

	Application::Application() :m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		BL_CORE_ASSERT(!s_Instance, "Instance already exist!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BL_BIND_EVENT_FNC(Application::OnEvent));
		m_Imgui = new ImguiLayer();
		PushOverLayer(m_Imgui);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f,0.2f,0.4f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f,0.5f,0.7f,0.5f,1.0f,
			 0.0f, 0.5f,0.0f,0.8f,0.2f,0.3f,1.0f
		};

		std::shared_ptr<VertexBuffer> tranVB(VertexBuffer::CreatVertextBuffer(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::Float3,"a_Position",false},
			{ShaderDataType::Float4,"a_Color",false}
		};
		tranVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(tranVB);

		unsigned int indices[3] = {
			0,1,2
		};
		std::shared_ptr<IndexBuffer> tranIB(IndexBuffer::CreatIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(tranIB);

		m_SquaVertexArray.reset(VertexArray::Create());

		float squaVertices[3 * 4] = {
			-0.7f,-0.7f,0.0f,
			-0.7f, 0.7f,0.0f,
			 0.7f, 0.7f,0.0f,
			 0.7f,-0.7f,0.0f,
		};

		std::shared_ptr<VertexBuffer> squaVB(VertexBuffer::CreatVertextBuffer(squaVertices, sizeof(squaVertices)));
		BufferLayout squaLayout = {
			{ShaderDataType::Float3,"a_Position",false}
		};
		squaVB->SetLayout(squaLayout);
		m_SquaVertexArray->AddVertexBuffer(squaVB);

		unsigned int squaIndices[6] = {
			0,1,2,
			0,2,3
		};
		std::shared_ptr<IndexBuffer> squaIB(IndexBuffer::CreatIndexBuffer(squaIndices, sizeof(squaIndices) / sizeof(uint32_t)));
		m_SquaVertexArray->SetIndexBuffer(squaIB);

		std::string vertexSrc = R"(
           #version 330
           layout(location = 0) in vec3 a_Position;
           layout(location = 1) in vec4 a_Color;

           out vec3 o_Position;
           out vec4 o_Color;
           uniform mat4 u_ViewProjection;

           void main(){
                 gl_Position=u_ViewProjection * vec4(a_Position,1.0f);
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

		std::string blueVertexSrc = R"(
           #version 330
           layout(location = 0) in vec3 a_Position;

           out vec3 o_Position;
           uniform mat4 u_ViewProjection;

           void main(){
                 gl_Position=u_ViewProjection * vec4(a_Position,1.0f);
           }
        )";

		std::string blueFragmentSrc = R"(
           #version 330
           layout(location = 0) out vec4 color;

           void main(){
                 color=vec4(0.2, 0.3, 0.5, 1.0);
           }
        )";

		m_BlueShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));

		m_Camera.SetPosition(glm::vec3(-0.2f, -0.2f, 0.0f));
		m_Camera.SetRotation(0.0f);
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

			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);
			Renderer::Submit(m_BlueShader, m_SquaVertexArray);
			Renderer::EndScene();

			Renderer::BeginScene(m_Camera);
			Renderer::Submit(m_Shader, m_VertexArray);
			Renderer::EndScene();

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

