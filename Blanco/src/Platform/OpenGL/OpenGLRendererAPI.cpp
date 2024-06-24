#include "BLpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/gl.h"

namespace Blanco
{
	void OpenGLRendererAPI::Init()
	{
		BL_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(float x, float y, float width, float height)
	{
		BL_PROFILE_FUNCTION();

		glViewport((int)x, (int)y, (int)width, (int)height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		BL_PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		BL_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		BL_PROFILE_FUNCTION();
		uint32_t indicesCount = count == 0 ? vertexArray->GetIndexBuffer()->GetCount() : count;
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
	}

}

