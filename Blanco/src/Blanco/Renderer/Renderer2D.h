#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Blanco
{
	class Renderer2D {
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(OrthoGraphicCamera& camera);
		static void EndScene();
		static void Flush();
		static void FlushAndReset();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		struct Statistics {
			uint32_t DrawCalls;
			uint32_t DrawQuads;
			uint32_t DrawVertices;
			uint32_t DrawIndices;
		};
		static Statistics& GetStats();
		static void ResetStats();
	};
}