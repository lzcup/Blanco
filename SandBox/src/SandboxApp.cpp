#include <Blanco.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"

class ExampleLayer :public Blanco::Layer {
public:
	ExampleLayer() :Layer("Example"), m_CameraController(1280.0f / 720.0f) {
		m_VertexArray = Blanco::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f,0.2f,0.4f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f,0.5f,0.7f,0.5f,1.0f,
			 0.0f, 0.5f,0.0f,0.8f,0.2f,0.3f,1.0f
		};

		Blanco::Ref<Blanco::VertexBuffer> tranVB(Blanco::VertexBuffer::CreatVertextBuffer(vertices, sizeof(vertices)));
		Blanco::BufferLayout layout = {
			{Blanco::ShaderDataType::Float3,"a_Position",false},
			{Blanco::ShaderDataType::Float4,"a_Color",false}
		};
		tranVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(tranVB);

		unsigned int indices[3] = {
			0,1,2
		};
		Blanco::Ref<Blanco::IndexBuffer> tranIB(Blanco::IndexBuffer::CreatIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(tranIB);

		m_SquaVertexArray = Blanco::VertexArray::Create();

		float squaVertices[5 * 4] = {
			-0.5f,-0.5f,0.0f,0.0f,0.0f,
			-0.5f, 0.5f,0.0f,0.0f,1.0f,
			 0.5f, 0.5f,0.0f,1.0f,1.0f,
			 0.5f,-0.5f,0.0f,1.0f,0.0f
		};

		Blanco::Ref<Blanco::VertexBuffer> squaVB(Blanco::VertexBuffer::CreatVertextBuffer(squaVertices, sizeof(squaVertices)));
		Blanco::BufferLayout squaLayout = {
			{Blanco::ShaderDataType::Float3,"a_Position",false},
			{Blanco::ShaderDataType::Float2,"a_TexCoord",false}
		};
		squaVB->SetLayout(squaLayout);
		m_SquaVertexArray->AddVertexBuffer(squaVB);

		unsigned int squaIndices[6] = {
			0,1,2,
			0,2,3
		};
		Blanco::Ref<Blanco::IndexBuffer> squaIB(Blanco::IndexBuffer::CreatIndexBuffer(squaIndices, sizeof(squaIndices) / sizeof(uint32_t)));
		m_SquaVertexArray->SetIndexBuffer(squaIB);

		std::string vertexSrc = R"(
           #version 330
           layout(location = 0) in vec3 a_Position;
           layout(location = 1) in vec4 a_Color;

           out vec3 o_Position;
           out vec4 o_Color;
           uniform mat4 u_ViewProjection;
           uniform mat4 u_Transform;

           void main(){
                 gl_Position=u_ViewProjection * u_Transform * vec4(a_Position,1.0f);
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

		m_ShaderLibrary.Load("VertexPosTriangle", vertexSrc, fragmentSrc);

		std::string flatColorVertexSrc = R"(
           #version 330
           layout(location = 0) in vec3 a_Position;

           out vec3 o_Position;
           uniform mat4 u_ViewProjection;
           uniform mat4 u_Transform;

           void main(){
                 gl_Position=u_ViewProjection * u_Transform * vec4(a_Position,1.0f);
           }
        )";

		std::string flatColorFragmentSrc = R"(
           #version 330
           layout(location = 0) out vec4 color;

           uniform vec3 u_FlatColor;

           void main(){
                 color=vec4(u_FlatColor,1.0f);
           }
        )";

		m_ShaderLibrary.Load("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);

		auto texture2DShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		std::dynamic_pointer_cast<Blanco::OpenGLShader>(texture2DShader)->Bind();
		std::dynamic_pointer_cast<Blanco::OpenGLShader>(texture2DShader)->UploadUniformInt("u_Texture", 0);
		m_Texture2D = Blanco::Texture2D::Create("assets/textures/checkerboard.png");
		m_Cat = Blanco::Texture2D::Create("assets/textures/cat.png");
	};
	~ExampleLayer() {};

	virtual void OnUpdate(Blanco::TimeStep ts) override
	{
		//Camera Control
		m_CameraController.OnUpdate(ts);

		Blanco::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Blanco::RenderCommand::Clear();

		Blanco::Renderer::BeginScene(m_CameraController.GetCamera());

		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
		std::dynamic_pointer_cast<Blanco::OpenGLShader>(flatColorShader)->Bind();
		std::dynamic_pointer_cast<Blanco::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_FlatColor", m_FlatColor);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.11f * x, 0.11f * y, 0.0f)) * scale;
				Blanco::Renderer::Submit(flatColorShader, m_SquaVertexArray, transform);
			}
		}

		m_Texture2D->Bind();
		auto texture2DShader = m_ShaderLibrary.Get("Texture");
		Blanco::Renderer::Submit(texture2DShader, m_SquaVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Cat->Bind();
		Blanco::Renderer::Submit(texture2DShader, m_SquaVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//Triangle
		//Blanco::Renderer::Submit(m_Shader, m_VertexArray);

		Blanco::Renderer::EndScene();



		
	}

	virtual void OnImguiRender() override {
		ImGui::Begin("Color test!");
		ImGui::ColorEdit3("Square Color",&m_FlatColor.r);
		ImGui::End();
	};

	virtual void OnEvent(Blanco::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	Blanco::ShaderLibrary m_ShaderLibrary;
	Blanco::Ref<Blanco::VertexArray> m_VertexArray;
	Blanco::Ref<Blanco::VertexArray> m_SquaVertexArray;
	Blanco::Ref<Blanco::Texture2D> m_Texture2D;
	Blanco::Ref<Blanco::Texture2D> m_Cat;
	Blanco::OrthoGraphicCameraController m_CameraController;

	glm::vec3 m_FlatColor = { 0.2f, 0.3f, 0.5f };
};


class SandboxApp :public Blanco::Application {
public:
	SandboxApp() { 
		PushLayer(std::make_shared<ExampleLayer>());
	};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp();
}
