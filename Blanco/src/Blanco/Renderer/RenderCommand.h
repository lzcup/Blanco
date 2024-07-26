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
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) {
			s_RendererAPI->DrawIndexed(vertexArray,count);
		};
		inline static void DrawLine(const Ref<VertexArray>& vertexArray, uint32_t count = 0) {
			s_RendererAPI->DrawLine(vertexArray, count);
		};
		inline static void SetLineWidth(float width) {
			s_RendererAPI->SetLineWidth(width);
		};
	private:
		static Ref<RendererAPI> s_RendererAPI;
	};
}