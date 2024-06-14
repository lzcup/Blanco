#include "BLpch.h"
#include "Renderer2D.h"
#include "gtc/matrix_transform.hpp"

namespace Blanco
{
	struct Renderer2DStorage{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage;

		s_Data->QuadVertexArray = VertexArray::Create();

		float squaVertices[3 * 4] = {
			 -0.5f,-0.5f,0.0f,
			 -0.5f, 0.5f,0.0f,
			  0.5f, 0.5f,0.0f,
			  0.5f,-0.5f,0.0f
		};

		Ref<VertexBuffer> squaVB(VertexBuffer::CreatVertextBuffer(squaVertices, sizeof(squaVertices)));
		BufferLayout squaLayout = {
			{ShaderDataType::Float3,"a_Position",false},
		};
		squaVB->SetLayout(squaLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squaVB);

		unsigned int squaIndices[6] = {
			0,1,2,
			0,2,3
		};
		Ref<IndexBuffer> squaIB(IndexBuffer::CreatIndexBuffer(squaIndices, sizeof(squaIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squaIB);

		s_Data->FlatColorShader=Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(OrthoGraphicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, color, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		s_Data->FlatColorShader->Bind();
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0,0,1 }) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);
		s_Data->FlatColorShader->SetFloat4("u_FlatColor", color);
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}

