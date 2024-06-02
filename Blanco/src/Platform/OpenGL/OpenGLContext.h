#pragma once
#include "Blanco/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Blanco
{
	class OpenGLContext :public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowhandle);

		virtual void Init() override;
		virtual void SwapBuffer() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}