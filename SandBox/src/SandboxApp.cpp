#include <Blanco.h>
#include "imgui.h"

class ExampleLayer :public Blanco::Layer {
public:
	ExampleLayer() :Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_Position(0.0f) {
		m_VertexArray.reset(Blanco::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f,0.2f,0.4f,0.8f,1.0f,
			 0.5f,-0.5f,0.0f,0.5f,0.7f,0.5f,1.0f,
			 0.0f, 0.5f,0.0f,0.8f,0.2f,0.3f,1.0f
		};

		std::shared_ptr<Blanco::VertexBuffer> tranVB(Blanco::VertexBuffer::CreatVertextBuffer(vertices, sizeof(vertices)));
		Blanco::BufferLayout layout = {
			{Blanco::ShaderDataType::Float3,"a_Position",false},
			{Blanco::ShaderDataType::Float4,"a_Color",false}
		};
		tranVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(tranVB);

		unsigned int indices[3] = {
			0,1,2
		};
		std::shared_ptr<Blanco::IndexBuffer> tranIB(Blanco::IndexBuffer::CreatIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(tranIB);

		m_SquaVertexArray.reset(Blanco::VertexArray::Create());

		float squaVertices[3 * 4] = {
			-0.7f,-0.7f,0.0f,
			-0.7f, 0.7f,0.0f,
			 0.7f, 0.7f,0.0f,
			 0.7f,-0.7f,0.0f,
		};

		std::shared_ptr<Blanco::VertexBuffer> squaVB(Blanco::VertexBuffer::CreatVertextBuffer(squaVertices, sizeof(squaVertices)));
		Blanco::BufferLayout squaLayout = {
			{Blanco::ShaderDataType::Float3,"a_Position",false}
		};
		squaVB->SetLayout(squaLayout);
		m_SquaVertexArray->AddVertexBuffer(squaVB);

		unsigned int squaIndices[6] = {
			0,1,2,
			0,2,3
		};
		std::shared_ptr<Blanco::IndexBuffer> squaIB(Blanco::IndexBuffer::CreatIndexBuffer(squaIndices, sizeof(squaIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Blanco::Shader(vertexSrc, fragmentSrc));

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

		m_BlueShader.reset(new Blanco::Shader(blueVertexSrc, blueFragmentSrc));
	};
	~ExampleLayer() {};

	virtual void OnUpdate() override
	{
		if (Blanco::Input::IsKeyPressed(BL_KEY_UP))
			m_Position.y += m_MoveSpeed;
		else if (Blanco::Input::IsKeyPressed(BL_KEY_DOWN))
			m_Position.y -= m_MoveSpeed;

		if (Blanco::Input::IsKeyPressed(BL_KEY_RIGHT))
			m_Position.x += m_MoveSpeed;
		else if (Blanco::Input::IsKeyPressed(BL_KEY_LEFT))
			m_Position.x -= m_MoveSpeed;

		if (Blanco::Input::IsKeyPressed(BL_KEY_A))
			m_Rotation -= m_RotateSpeed;
		else if (Blanco::Input::IsKeyPressed(BL_KEY_D))
			m_Rotation += m_RotateSpeed;

		m_Camera.SetPosition(m_Position);
		m_Camera.SetRotation(m_Rotation);

		Blanco::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Blanco::RenderCommand::Clear();

		Blanco::Renderer::BeginScene(m_Camera);
		Blanco::Renderer::Submit(m_BlueShader, m_SquaVertexArray);
		Blanco::Renderer::EndScene();

		Blanco::Renderer::BeginScene(m_Camera);
		Blanco::Renderer::Submit(m_Shader, m_VertexArray);
		Blanco::Renderer::EndScene();
	}

	virtual void OnImguiRender() override {
		
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
	std::shared_ptr<Blanco::Shader> m_Shader;
	std::shared_ptr<Blanco::Shader> m_BlueShader;
	std::shared_ptr<Blanco::VertexArray> m_VertexArray;
	std::shared_ptr<Blanco::VertexArray> m_SquaVertexArray;
	Blanco::OrthoGraphicCamera m_Camera;
	glm::vec3 m_Position;
	float m_MoveSpeed = 0.1f;
	float m_Rotation = 0.0f;
	float m_RotateSpeed = 1.0f;
	glm::vec4 m_Projection = { -1.6f, 1.6f, -0.9f, 0.9f };
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
