#include "BLpch.h"
#include "Renderer2D.h"
#include "Blanco/Renderer/UniformBuffer.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace Blanco
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 Texcoord;
		float TextureSlot;
		float TilingFactor;

		//Editor-only
		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		//Editor-only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		//Editor-only
		int EntityID;
	};

	struct Renderer2DData {
		static const uint32_t MAXQUAD = 10000;
		static const uint32_t MAXINDICES = MAXQUAD * 6;
		static const uint32_t MAXVERTICES = MAXQUAD * 4;
		static const uint32_t MAXTEXTURESLOT = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;
		Ref<VertexBuffer> QuadVertexBuffer;

		Ref<VertexArray> CircleVertexArray;
		Ref<Shader> CircleShader;
		Ref<VertexBuffer> CircleVertexBuffer;

		Ref<VertexArray> LineVertexArray;
		Ref<Shader> LineShader;
		Ref<VertexBuffer> LineVertexBuffer;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 0.5f;

		uint32_t TextureIndex = 1;
		std::array<Ref<Texture2D>, MAXTEXTURESLOT> TextureSlots;

		glm::vec4 QuadVerticesPosition[4] = { glm::vec4(0.0f),glm::vec4(0.0f),glm::vec4(0.0f),glm::vec4(0.0f) };

		Renderer2D::Statistics Stats = { 0,0,0,0 };
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		BL_PROFILE_FUNCTION();

		//Quad Vertex
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::CreatVertextBuffer(sizeof(QuadVertex) * s_Data.MAXVERTICES);
		BufferLayout quadLayout = {
			{ShaderDataType::Float3,"a_Position",false},
			{ShaderDataType::Float4,"a_Color",false},
			{ShaderDataType::Float2,"a_TexCoord",false},
			{ShaderDataType::Float,"a_TexSlot",false},
			{ShaderDataType::Float,"a_TilingFactor",false},
			{ShaderDataType::Int,"a_EntityID",true }
		};
		s_Data.QuadVertexBuffer->SetLayout(quadLayout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MAXVERTICES];

		//Circle Vertex
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::CreatVertextBuffer(sizeof(CircleVertex) * s_Data.MAXVERTICES);
		BufferLayout circleLayout = {
			{ShaderDataType::Float3,"a_WorldPosition",false},
			{ShaderDataType::Float3,"a_LocalPosition",false},
			{ShaderDataType::Float4,"a_Color",false},
			{ShaderDataType::Float,"a_Thickness",false},
			{ShaderDataType::Float,"a_Fade",false},
			{ShaderDataType::Int,"a_EntityID",true }
		};
		s_Data.CircleVertexBuffer->SetLayout(circleLayout);
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);

		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MAXVERTICES];

		//Line Vertex
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::CreatVertextBuffer(sizeof(LineVertex) * s_Data.MAXVERTICES);
		BufferLayout lineLayout = {
			{ShaderDataType::Float3,"a_Position",false},
			{ShaderDataType::Float4,"a_Color",false},
			{ShaderDataType::Int,"a_EntityID",true }
		};
		s_Data.LineVertexBuffer->SetLayout(lineLayout);
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MAXVERTICES];

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
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); //Use QuadIB
		delete[] quadIndices;

		//Quad Shader
		int sampler2D[s_Data.MAXTEXTURESLOT];
		for (uint32_t i = 0; i < s_Data.MAXTEXTURESLOT; i++)
			sampler2D[i] = i;
		s_Data.QuadShader = Shader::Create("assets/shaders/Render2D_Quad.glsl");
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", sampler2D, s_Data.MAXTEXTURESLOT);

		//Circle Shader
		s_Data.CircleShader = Shader::Create("assets/shaders/Render2D_Circle.glsl");
		s_Data.CircleShader->Bind();

		//Line Shader
		s_Data.LineShader = Shader::Create("assets/shaders/Render2D_Line.glsl");
		s_Data.LineShader->Bind();
		
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

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::BeginScene(EditorCamera& camera)
	{
		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjection", viewProj);

		SetLineWidth(s_Data.LineWidth);
		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		BL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount) 
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(dataSize, s_Data.QuadVertexBufferBase);
			s_Data.QuadShader->Bind();
			for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}
		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(dataSize, s_Data.CircleVertexBufferBase);
			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}
		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(dataSize, s_Data.LineVertexBufferBase);
			s_Data.LineShader->Bind();
			RenderCommand::DrawLine(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
		
	}

	void Renderer2D::NextBatch()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

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
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,int entityID)
	{
		if (s_Data.QuadIndexCount + s_Data.CircleIndexCount >= s_Data.MAXINDICES)
			NextBatch();

		float textureIndex = 0.0f;
		float tilingFactor = 1.0f;

		glm::vec2 texCoords[4] = { {0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

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

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor, int entityID)
	{
		if ((s_Data.QuadIndexCount + s_Data.CircleIndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			s_Data.TextureIndex++;
		}

		glm::vec2 texCoords[4] = { {0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f} };

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Texcoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureSlot = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, subTexture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		if ((s_Data.QuadIndexCount + s_Data.CircleIndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			NextBatch();

		Ref <Texture2D> texture = subTexture->GetTexture();
		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
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

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		BL_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount + s_Data.CircleIndexCount >= s_Data.MAXINDICES)
			NextBatch();

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

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, texture, color, tilingFactor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor)
	{
		BL_PROFILE_FUNCTION();

		if ((s_Data.QuadIndexCount + s_Data.CircleIndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
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

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		DrawRotateQuad(glm::vec3(position, 0.0f), rotation, size, subTexture, color, tilingFactor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color /*= glm::vec4(1.0f)*/, float tilingFactor /*= 1.0f*/)
	{
		if ((s_Data.QuadIndexCount + s_Data.CircleIndexCount >= s_Data.MAXINDICES) || (s_Data.TextureIndex >= s_Data.MAXTEXTURESLOT))
			NextBatch();

		Ref <Texture2D> texture = subTexture->GetTexture();
		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureIndex; i++) {
			if (*texture.get() == *s_Data.TextureSlots[i].get()) {
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureIndex;
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
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

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		if (s_Data.QuadIndexCount + s_Data.CircleIndexCount >= s_Data.MAXINDICES)
			NextBatch();

		for (uint32_t i = 0; i < 4; i++) {
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVerticesPosition[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVerticesPosition[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.DrawQuads++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x / 2, position.y - size.y / 2, position.z);
		glm::vec3 p1 = glm::vec3(position.x - size.x / 2, position.y + size.y / 2, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x / 2, position.y + size.y / 2, position.z);
		glm::vec3 p3 = glm::vec3(position.x + size.x / 2, position.y - size.y / 2, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 postions[4];
		for (int i = 0; i < 4; i++)
			postions[i] = transform * s_Data.QuadVerticesPosition[i];

		DrawLine(postions[0], postions[1], color, entityID);
		DrawLine(postions[1], postions[2], color, entityID);
		DrawLine(postions[2], postions[3], color, entityID);
		DrawLine(postions[3], postions[0], color, entityID);
	}						  

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		RenderCommand::SetLineWidth(width);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.Color, src.TilingFactor, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
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

