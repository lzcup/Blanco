#pragma once
#include "RendererAPI.h"

namespace Blanco
{
	class RenderCommand {
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		};
		inline static void SetViewport(float x, float y, float width, float height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear() {
			s_RendererAPI->Clear();
		};
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		};
	private:
		static RendererAPI* s_RendererAPI;
	};
}