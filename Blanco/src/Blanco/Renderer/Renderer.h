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

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
	private:
		struct SceneData{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};
}