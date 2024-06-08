#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"

namespace Blanco
{
	class Renderer {
	public:
		static void BeginScene(OrthoGraphicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
	private:
		struct SceneData{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};
}