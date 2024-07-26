#pragma once
#include "glm.hpp"
#include "VertexArray.h"

namespace Blanco
{
	class RendererAPI {
	public:
		enum class API {
			NONE = 0,
			OPENGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(float x, float y, float width, float height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;
		virtual void DrawLine(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;
		virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}