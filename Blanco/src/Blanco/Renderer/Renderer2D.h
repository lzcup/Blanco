#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"

namespace Blanco
{
	class Renderer2D {
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(OrthoGraphicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
	};
}