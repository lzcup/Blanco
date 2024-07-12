#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"
#include "EditorCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Blanco/Scene/Components.h"

namespace Blanco
{
	class Renderer2D {
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(Camera& camera,const glm::mat4& transform);
		static void BeginScene(EditorCamera& camera);
		static void BeginScene(OrthoGraphicCamera& camera);//To remove
		static void EndScene();
		static void Flush();
		static void FlushAndReset();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);


		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void DrawSprite(const glm::mat4& transform, const SpriteComponent& src, int entityID = -1);

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