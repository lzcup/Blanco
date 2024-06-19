#include "BLpch.h"
#include "Renderer2D.h"
#include "gtc/matrix_transform.hpp"

namespace Blanco
{
	struct Renderer2DStorage{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		BL_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage;

		s_Data->QuadVertexArray = VertexArray::Create();

		float squaVertices[5 * 4] = {
			-0.5f,-0.5f,0.0f,0.0f,0.0f,
			-0.5f, 0.5f,0.0f,0.0f,1.0f,
			 0.5f, 0.5f,0.0f,1.0f,1.0f,
			 0.5f,-0.5f,0.0f,1.0f,0.0f
		};

		Ref<VertexBuffer> squaVB(VertexBuffer::CreatVertextBuffer(squaVertices, sizeof(squaVertices)));
		BufferLayout squaLayout = {
			{ShaderDataType::Float3,"a_Position",false},
			{ShaderDataType::Float2,"a_TexCoord",false}
		};
		squaVB->SetLayout(squaLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squaVB);

		unsigned int squaIndices[6] = {
			0,1,2,
			0,2,3
		};
		Ref<IndexBuffer> squaIB(IndexBuffer::CreatIndexBuffer(squaIndices, sizeof(squaIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squaIB);

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data->WhiteTexture->SetData(&data, sizeof(data));
	}

	void Renderer2D::ShutDown()
	{
		BL_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(OrthoGraphicCamera& camera)
	{
		BL_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		BL_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		BL_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_TextureColor", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		
		s_Data->WhiteTexture->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, texture, color, tilingFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		BL_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data->TextureShader->SetFloat4("u_TextureColor", color);

		texture->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		BL_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0,0,1 }) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data->TextureShader->SetFloat4("u_TextureColor", color);

		s_Data->WhiteTexture->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, texture, color, tilingFactor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		BL_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0,0,1 }) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data->TextureShader->SetFloat4("u_TextureColor", color);

		texture->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}

