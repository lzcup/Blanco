#pragma once
#include "Blanco/Renderer/RendererAPI.h"

namespace Blanco
{
	class OpenGLRendererAPI :public RendererAPI {
	public:
		OpenGLRendererAPI() {};

		virtual void Init();
		virtual void SetViewport(float x, float y, float width, float height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) override;
		virtual void DrawLine(const Ref<VertexArray>& vertexArray, uint32_t count = 0) override;
		virtual void SetLineWidth(float width) override;
	};
}