#include <Blanco.h>
#include "imgui.h"
#include "gtc/matrix_transform.hpp"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer :public Blanco::Layer {
public:
	ExampleLayer() :Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f){
		m_VertexArray.reset(Blanco::VertexArray::Create());

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

		m_SquaVertexArray.reset(Blanco::VertexArray::Create());

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

		m_Shader.reset(Blanco::Shader::Create(vertexSrc, fragmentSrc));

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

		m_FlatColorShader.reset(Blanco::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));

		std::string texture2DVertexSrc = R"(
           #version 330
           layout(location = 0) in vec3 a_Position;
           layout(location = 1) in vec2 a_TexCoord;

           out vec2 v_TexCoord;
           uniform mat4 u_ViewProjection;
           uniform mat4 u_Transform;

           void main(){
                 gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0f);
                 v_TexCoord = a_TexCoord;
           }
        )";

		std::string texture2DFragmentSrc = R"(
           #version 330
           layout(location = 0) out vec4 color;

           in vec2 v_TexCoord;
           uniform vec3 u_FlatColor;
           uniform sampler2D u_Texture;

           void main(){
                 color=texture(u_Texture,v_TexCoord);
           }
        )";

		m_Texture2DShader.reset(Blanco::Shader::Create(texture2DVertexSrc, texture2DFragmentSrc));
		std::dynamic_pointer_cast<Blanco::OpenGLShader>(m_Texture2DShader)->Bind();
		std::dynamic_pointer_cast<Blanco::OpenGLShader>(m_Texture2DShader)->UploadUniformInt("u_Texture", 0);
		m_Texture2D = Blanco::Texture2D::Create("asset/textures/checkerboard.png");
		m_Cat = Blanco::Texture2D::Create("asset/textures/cat.png");
	};
	~ExampleLayer() {};

	virtual void OnUpdate(Blanco::TimeStep ts) override
	{
		//Camera Control
		if (Blanco::Input::IsKeyPressed(BL_KEY_UP))
			m_CameraPosition.y += m_MoveSpeed * ts;
		else if (Blanco::Input::IsKeyPressed(BL_KEY_DOWN))
			m_CameraPosition.y -= m_MoveSpeed * ts;

		if (Blanco::Input::IsKeyPressed(BL_KEY_RIGHT))
			m_CameraPosition.x += m_MoveSpeed * ts;
		else if (Blanco::Input::IsKeyPressed(BL_KEY_LEFT))
			m_CameraPosition.x -= m_MoveSpeed * ts;

		if (Blanco::Input::IsKeyPressed(BL_KEY_A))
			m_Rotation -= m_RotateSpeed * ts;
		else if (Blanco::Input::IsKeyPressed(BL_KEY_D))
			m_Rotation += m_RotateSpeed * ts;

		if (Blanco::Input::IsKeyPressed(BL_KEY_A))
			m_Rotation -= m_RotateSpeed * ts;
		else if (Blanco::Input::IsKeyPressed(BL_KEY_D))
			m_Rotation += m_RotateSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_Rotation);

		Blanco::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Blanco::RenderCommand::Clear();

		Blanco::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Blanco::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Blanco::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_FlatColor", m_FlatColor);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.11f * x, 0.11f * y, 0.0f)) * scale;
				Blanco::Renderer::Submit(m_FlatColorShader, m_SquaVertexArray, transform);
			}
		}

		m_Texture2D->Bind();
		Blanco::Renderer::Submit(m_Texture2DShader, m_SquaVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Cat->Bind();
		Blanco::Renderer::Submit(m_Texture2DShader, m_SquaVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
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
		Blanco::Dispatcher dispatcher(event);
		dispatcher.Dispatch<Blanco::MouseScrolledEvent>(BL_BIND_EVENT_FNC(ExampleLayer::OnMouseScolledEvent));
	}
private:
	bool OnMouseScolledEvent(Blanco::MouseScrolledEvent& event) {
		if (event.GetYOffset() == 1.0f) {
			m_Projection.x *= 1.1f;
			m_Projection.y *= 1.1f;
			m_Projection.z *= 1.1f;
			m_Projection.w *= 1.1f;
			Blanco::OrthoGraphicCamera newCamera(m_Projection.x, m_Projection.y, m_Projection.z, m_Projection.w);
			m_Camera = newCamera;
		}
		if (event.GetYOffset() == -1.0f) {
			m_Projection.x *= (float)10 / 11;
			m_Projection.y *= (float)10 / 11;
			m_Projection.z *= (float)10 / 11;
			m_Projection.w *= (float)10 / 11;
			Blanco::OrthoGraphicCamera newCamera(m_Projection.x, m_Projection.y, m_Projection.z, m_Projection.w);
			m_Camera = newCamera;
		}
		return false;
	}
private:
	Blanco::Ref<Blanco::Shader> m_Shader;
	Blanco::Ref<Blanco::Shader> m_FlatColorShader;
	Blanco::Ref<Blanco::Shader> m_Texture2DShader;
	Blanco::Ref<Blanco::VertexArray> m_VertexArray;
	Blanco::Ref<Blanco::VertexArray> m_SquaVertexArray;
	Blanco::Ref<Blanco::Texture2D> m_Texture2D;
	Blanco::Ref<Blanco::Texture2D> m_Cat;
	Blanco::OrthoGraphicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_MoveSpeed = 5.0f;
	float m_Rotation = 0.0f;
	float m_RotateSpeed = 180.0f;
	glm::vec4 m_Projection = { -1.6f, 1.6f, -0.9f, 0.9f };

	glm::vec3 m_FlatColor = { 0.2f, 0.3f, 0.5f };
};


class SandboxApp :public Blanco::Application {
public:
	SandboxApp() { 
		PushLayer(new ExampleLayer());
	};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp();
}
