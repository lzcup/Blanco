#include "BLpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Blanco
{
	Ref<RendererAPI> RenderCommand::s_RendererAPI = CreateRef<OpenGLRendererAPI>();
}