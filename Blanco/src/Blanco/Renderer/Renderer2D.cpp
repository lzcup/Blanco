#include "BLpch.h"
#include "Renderer2D.h"
#include "gtc/matrix_transform.hpp"

namespace Blanco
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 Texcoord;
		float TextureSlot;
		float TilingFactor;
	};

	struct Renderer2DData {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		Ref<VertexBuffer> QuadVertexBuffer;

		static const uint32_t MAXQUAD = 10000;
		static const uint32_t MAXINDICES = MAXQUAD * 6;
		static const uint32_t MAXVERTICES = MAXQUAD * 4;
		static const uint32_t MAXTEXTURESLOT = 32;

		uint32_t IndexCount = 0;
		uint32_t TextureIndex = 1;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MAXTEXTURESLOT> TextureSlots;

		glm::vec4 QuadVerticesPosition[4] = { glm::vec4(0.0f),glm::vec4(0.0f),glm::vec4(0.0f),glm::vec4(0.0f) };

		Renderer2D::Statistics Stats = { 0,0,0,0 };
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		BL_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::CreatVertextBuffer(sizeof(QuadVertex) * s_Data.MAXVERTICES);
		BufferLayout quadLayout = {
			{ShaderDataType::Float3,"a_Position",false},
			{ShaderDataType::Float4,"a_Color",false},
			{ShaderDataType::Float2,"a_TexCoord",false},
			{ShaderDataType::Float,"a_TexSlot",false},
			{ShaderDataType::Float,"a_TilingFactor",false}
		};
		s_Data.QuadVertexBuffer->SetLayout(quadLayout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MAXVERTICES];

		uint32_t* quadIndices = new uint32_t[s_Data.MAXINDICES];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MAXINDICES; i += 6) {
			quadIndices[i] = offset;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset;
			quadIndices[i + 4] = offset + 2;
			quadIndices[i + 5] = offset + 3;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB(IndexBuffer::CreatIndexBuffer(quadIndices, s_Data.MAXINDICES));
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		int sampler2D[s_Data.MAXTEXTURESLOT];
		for (uint32_t i = 0; i < s_Data.MAXTEXTURESLOT; i++)
			sampler2D[i] = i;
		s_Data.TextureShader->SetIntArray("u_Texture", sampler2D, s_Data.MAXTEXTURESLOT);

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data.WhiteTexture->SetData(&data, sizeof(data));

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVerticesPosition[0] = { -0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVerticesPosition[1] = { -0.5f, 0.5f,0.0f,1.0f };
		s_Data.QuadVerticesPosition[2] = { 0.5f, 0.5f,0.0f,1.0f };
		s_Data.QuadVerticesPosition[3] = { 0.5f,-0.5f,0.0f,1.0f };

	}

	void Renderer2D::ShutDown()
	{
		BL_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(Camera& camera, const glm::mat4& transform)
	{
		BL_PROFILE_FUNCTION();

		glm::mat4 viewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProjectionMatrix);

		s_Data.IndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::BeginScene(EditorCamera& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		s_Data.IndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::BeginScene(OrthoGraphicCamera& camera)
	{
		BL_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.IndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		BL_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(dataSize, s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexArray->Bind();
		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.IndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.IndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		DrawQuad(transform, color);

	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (s_Data.IndexCount >= s_Data.MAXINDICES)
			FlushAndReset();

		float textureIndex = 0.0f;
		float tilingFactor = 1.0f;

		glm::vec2 texCoords[4] = { {0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.IndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, texture, color, tilingFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		DrawQuad(transform, texture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		if ((s_Data.IndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureIndex++;
		}

		glm::vec2 texCoords[4] = { {0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.IndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, subTexture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		if ((s_Data.IndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			FlushAndReset();

		Ref <Texture2D> texture = subTexture->GetTexture();
		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureIndex++;
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		const glm::vec4& coords = subTexture->GetCoords();
		glm::vec2 texCoords[4] = { {coords.x,coords.y},{coords.x,coords.w},{coords.z,coords.w},{coords.z,coords.y} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.IndexCount += 6;
		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		BL_PROFILE_FUNCTION();

		if (s_Data.IndexCount >= s_Data.MAXINDICES)
			FlushAndReset();

		float textureIndex = 0.0f;
		float tilingFactor = 1.0f;
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0,0,1 }) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::vec2 texCoords[4] = { {0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.IndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, texture, color, tilingFactor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		BL_PROFILE_FUNCTION();

		if ((s_Data.IndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureIndex++;
			BL_CORE_ASSERT(s_Data.TextureIndex < s_Data.MAXTEXTURESLOT, "Beyond max texture slot");
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0,0,1 }) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::vec2 texCoords[4] = { {0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.IndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, subTexture, color, tilingFactor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		if ((s_Data.IndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			FlushAndReset();

		Ref <Texture2D> texture = subTexture->GetTexture();
		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureIndex++;
			BL_CORE_ASSERT(s_Data.TextureIndex < s_Data.MAXTEXTURESLOT, "Beyond max texture slot");
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0,0,1 }) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		const glm::vec4& coords = subTexture->GetCoords();
		glm::vec2 texCoords[4] = { {coords.x,coords.y},{coords.x,coords.w},{coords.z,coords.w},{coords.z,coords.y} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.IndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}

	Renderer2D::Statistics& Renderer2D::GetStats()
	{
		s_Data.Stats.DrawVertices = s_Data.Stats.DrawQuads * 4;
		s_Data.Stats.DrawIndices = s_Data.Stats.DrawQuads * 6;
		return s_Data.Stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}

